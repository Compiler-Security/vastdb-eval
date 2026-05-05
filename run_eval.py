#!/usr/bin/env python3

from __future__ import annotations

import argparse
import concurrent.futures
import dataclasses
import datetime as dt
import json
import os
import re
import signal
import shutil
import socket
import subprocess
import sys
import threading
import time
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parent
TESTCASES_DIR = ROOT / "testcases"
OUTPUTS_DIR = ROOT / "outputs"
CONFIG_PATH = ROOT / "configs" / "env.json"
OPENCODE_CONFIG_PATHS = {
    "baseline": ROOT / "configs" / "opencode_baseline.json",
    "vastdb": ROOT / "configs" / "opencode_vastdb.json",
    "judge": ROOT / "configs" / "opencode_judge.json",
}
PROMPTS_DIR = ROOT / "prompts"

BASELINE_TEMPLATE = PROMPTS_DIR / "baseline.md"
VASTDB_TEMPLATE = PROMPTS_DIR / "vastdb.md"
JUDGE_TEMPLATE = PROMPTS_DIR / "judge.md"
JUDGE_SCHEMA = PROMPTS_DIR / "judge_schema.json"
OPENCODE_RUNNER = ROOT / "scripts" / "opencode_runner.mjs"
SUMMARY_PATH = OUTPUTS_DIR / "summary.json"
CUR_SUMMARY_PATH = OUTPUTS_DIR / "cur_summary.json"

DEFAULT_NEO4J_IMAGE = "neo4j:latest"
CONTAINER_PREFIX = "vastdb-eval"
PROGRESS_LOG_LOCK = threading.Lock()
EAST_8 = dt.timezone(dt.timedelta(hours=8))
VAST_BITCODE_FAILURE_MARKER = "Failed to generate bitcode"


class EvalError(Exception):
    def __init__(self, stage: str, message: str):
        super().__init__(message)
        self.stage = stage
        self.message = message


@dataclasses.dataclass(frozen=True)
class TestCase:
    cwd_id: int
    case_id: int

    @property
    def cwd_id_text(self) -> str:
        return str(self.cwd_id)

    @property
    def id_text(self) -> str:
        return str(self.case_id)

    @property
    def id2(self) -> str:
        return f"{self.case_id:02d}"

    @property
    def name(self) -> str:
        return f"CWD-{self.cwd_id}-{self.id2}"

    @property
    def src(self) -> Path:
        return TESTCASES_DIR / f"CWD-{self.cwd_id}" / self.name

    @property
    def work(self) -> Path:
        return OUTPUTS_DIR / f"CWD-{self.cwd_id}" / self.name

    @property
    def container(self) -> str:
        return f"{CONTAINER_PREFIX}-{self.cwd_id}-{self.id2}"

    @property
    def bolt_port(self) -> int:
        return 40000 + (self.cwd_id - 1000) * 100 + self.case_id

    @property
    def http_port(self) -> int:
        return 45000 + (self.cwd_id - 1000) * 100 + self.case_id


def now_iso() -> str:
    return dt.datetime.now(dt.timezone.utc).isoformat()


def now_east8_iso() -> str:
    return dt.datetime.now(EAST_8).isoformat()


def format_duration_seconds(seconds: float) -> str:
    total_seconds = max(0, int(round(seconds)))
    minutes, remainder = divmod(total_seconds, 60)
    return f"{minutes}m{remainder:02d}s"


def read_json(path: Path) -> dict[str, Any]:
    try:
        with path.open("r", encoding="utf-8") as f:
            data = json.load(f)
    except FileNotFoundError as exc:
        raise EvalError("config", f"missing file: {path}") from exc
    except json.JSONDecodeError as exc:
        raise EvalError("config", f"invalid JSON in {path}: {exc}") from exc
    if not isinstance(data, dict):
        raise EvalError("config", f"{path} must contain a JSON object")
    return data


def write_json(path: Path, data: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")


def log_progress(case: TestCase, stage: str, event: str) -> None:
    progress_log_path = OUTPUTS_DIR / "progress.log"
    line = f"{now_east8_iso()}\t{case.name}\t{stage}\t{event}\n"
    with PROGRESS_LOG_LOCK:
        progress_log_path.parent.mkdir(parents=True, exist_ok=True)
        with progress_log_path.open("a", encoding="utf-8") as f:
            f.write(line)


def reset_progress_log() -> None:
    progress_log_path = OUTPUTS_DIR / "progress.log"
    with PROGRESS_LOG_LOCK:
        progress_log_path.parent.mkdir(parents=True, exist_ok=True)
        progress_log_path.write_text("", encoding="utf-8")


def run_progress_stage(case: TestCase, stage: str, action: Any) -> Any:
    log_progress(case, stage, "start")
    try:
        return action()
    finally:
        log_progress(case, stage, "finish")


def load_env_config() -> dict[str, Any]:
    if not CONFIG_PATH.exists():
        raise EvalError("config", f"missing file: {CONFIG_PATH}")
    raw = read_json(CONFIG_PATH)
    cfg = {
        "neo4j": raw.get("neo4j", {}) or {},
        "vast": raw.get("vast", {}) or {},
        "neo4j_image": raw.get("neo4j_image", raw.get("NEO4J_IMAGE", DEFAULT_NEO4J_IMAGE)),
    }
    validate_env_config(cfg)
    return cfg


def validate_env_config(cfg: dict[str, Any]) -> None:
    for key in ("neo4j", "vast"):
        if not isinstance(cfg.get(key), dict):
            raise EvalError("config", f"configs/env.json field '{key}' must be an object")


def stringify_env(values: dict[str, Any]) -> dict[str, str]:
    return {str(k): str(v) for k, v in values.items() if v is not None}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run VAST DB evaluation test cases.")
    parser.add_argument("selector", help="'all' or CWD-ID, e.g. 1031")
    parser.add_argument("case", nargs="?", help="ID or range, e.g. 1 or 1..10")
    parser.add_argument("--jobs", type=int, default=1, help="number of test cases to run concurrently")
    return parser.parse_args()


def parse_case_id(value: str) -> int:
    if not re.fullmatch(r"\d+", value):
        raise EvalError("input", f"invalid test case id: {value}")
    return int(value, 10)


def discover_all_cases() -> list[TestCase]:
    cases: list[TestCase] = []
    pattern = re.compile(r"^CWD-(\d+)-(\d+)$")
    for path in sorted(TESTCASES_DIR.glob("CWD-*/CWD-*-*")):
        if not path.is_dir():
            continue
        match = pattern.match(path.name)
        if not match:
            continue
        cases.append(TestCase(int(match.group(1)), int(match.group(2))))
    return cases


def select_cases(args: argparse.Namespace) -> list[TestCase]:
    if args.selector == "all":
        if args.case is not None:
            raise EvalError("input", "'all' does not accept a case id argument")
        return discover_all_cases()

    if not re.fullmatch(r"\d+", args.selector):
        raise EvalError("input", f"invalid CWD-ID: {args.selector}")
    if args.case is None:
        raise EvalError("input", "missing case id or range")

    cwd_id = int(args.selector, 10)
    if ".." in args.case:
        start_text, end_text = args.case.split("..", 1)
        start = parse_case_id(start_text)
        end = parse_case_id(end_text)
        if end < start:
            raise EvalError("input", f"invalid range: {args.case}")
        return [TestCase(cwd_id, case_id) for case_id in range(start, end + 1)]

    return [TestCase(cwd_id, parse_case_id(args.case))]


def check_runtime_dependencies() -> None:
    missing: list[str] = []
    if shutil.which("node") is None:
        missing.append("node")
    if shutil.which("docker") is None:
        missing.append("docker")
    if shutil.which("cmake") is None:
        missing.append("cmake")

    if shutil.which("node") is not None:
        proc = run_process(["node", "-e", "import('@opencode-ai/sdk')"], ROOT)
        if proc.returncode != 0:
            missing.append("@opencode-ai/sdk")

    if missing:
        install = [
            "npm install",
            "install Docker and ensure `docker` is on PATH",
            "install CMake and ensure `cmake` is on PATH",
        ]
        raise EvalError(
            "dependencies",
            "missing runtime dependencies: "
            + ", ".join(missing)
            + ". Suggested installs: "
            + "; ".join(install),
        )


def copy_case_to_work(case: TestCase) -> None:
    if not case.src.is_dir():
        raise EvalError("prepare", f"test case directory does not exist: {case.src}")
    for required in ("cve.json", "bad", "good"):
        if not (case.src / required).exists():
            raise EvalError("prepare", f"test case is missing {required}: {case.src}")

    if case.work.exists():
        shutil.rmtree(case.work)
    case.work.mkdir(parents=True, exist_ok=True)
    shutil.copy2(case.src / "cve.json", case.work / "cve.json")
    for dirname in ("bad", "good"):
        shutil.copytree(case.src / dirname, case.work / dirname, dirs_exist_ok=True)


def parse_cve(case: TestCase) -> tuple[str, str]:
    cve_path = case.work / "cve.json"
    try:
        cve = read_json(cve_path)
    except EvalError as exc:
        raise EvalError("cve", exc.message) from exc

    cwe_id = cve.get("CWE-ID")
    if not isinstance(cwe_id, str) or not cwe_id.strip():
        raise EvalError("cve", f"missing or invalid CWE-ID in {cve_path}")

    if "source" in cve or "sink" in cve:
        parts = []
        if "source" in cve:
            parts.append(f"source:\n{cve['source']}")
        if "sink" in cve:
            parts.append(f"sink:\n{cve['sink']}")
        return cwe_id.strip(), "\n\n".join(parts)

    location = cve.get("location")
    if isinstance(location, str) and location.strip():
        return cwe_id.strip(), f"location:\n{location}"

    raise EvalError("cve", f"cve.json must contain source/sink or location: {cve_path}")


def render_template(path: Path, **values: str) -> str:
    try:
        template = path.read_text(encoding="utf-8")
    except FileNotFoundError as exc:
        raise EvalError("prompt", f"missing prompt template: {path}") from exc
    return template.format(**values)


def run_process(
    args: list[str],
    cwd: Path,
    env: dict[str, str] | None = None,
    timeout: int | None = None,
) -> subprocess.CompletedProcess[str]:
    merged_env = os.environ.copy()
    if env:
        merged_env.update(env)

    proc = subprocess.Popen(
        args,
        cwd=str(cwd),
        env=merged_env,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        start_new_session=True,
    )
    try:
        stdout, stderr = proc.communicate(timeout=timeout)
        kill_process_group(proc)
        return subprocess.CompletedProcess(args, proc.returncode, stdout, stderr)
    except subprocess.TimeoutExpired:
        kill_process_group(proc)
        stdout, stderr = proc.communicate()
        return subprocess.CompletedProcess(args, proc.returncode if proc.returncode is not None else -9, stdout, stderr)
    except KeyboardInterrupt:
        kill_process_group(proc)
        proc.communicate()
        raise


def kill_process_group(proc: subprocess.Popen[str]) -> None:
    try:
        os.killpg(proc.pid, signal.SIGTERM)
    except ProcessLookupError:
        return
    try:
        proc.wait(timeout=5)
        return
    except subprocess.TimeoutExpired:
        pass
    try:
        os.killpg(proc.pid, signal.SIGKILL)
    except ProcessLookupError:
        return
    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        pass


def run_cleanup_process(args: list[str], cwd: Path, timeout: int = 10) -> subprocess.CompletedProcess[str]:
    if threading.current_thread() is not threading.main_thread():
        return run_process(args, cwd, timeout=timeout)

    previous_handler = signal.getsignal(signal.SIGINT)
    try:
        signal.signal(signal.SIGINT, signal.SIG_IGN)
        return run_process(args, cwd, timeout=timeout)
    finally:
        signal.signal(signal.SIGINT, previous_handler)


def docker_inspect_running(container: str) -> bool | None:
    proc = run_process(["docker", "inspect", "-f", "{{.State.Running}}", container], ROOT)
    if proc.returncode != 0:
        return None
    return proc.stdout.strip().lower() == "true"


def wait_for_neo4j_ready(case: TestCase, timeout_seconds: int = 90) -> dict[str, Any]:
    started = time.monotonic()
    deadline = time.monotonic() + timeout_seconds
    attempts = 0
    cmd = [
        "docker",
        "logs",
        f"{case.container}",
    ]
    last_result: dict[str, Any] | None = None
    while time.monotonic() < deadline:
        attempts += 1
        proc = run_process(cmd, ROOT, timeout=5)
        last_result = {
            "returncode": proc.returncode,
            "stdout": proc.stdout,
            "stderr": proc.stderr,
        }
        if "Bolt enabled " in proc.stdout and proc.returncode == 0:
            return {
                "container": case.container,
                "bolt_port": case.bolt_port,
                "command": cmd,
                "timeout_seconds": timeout_seconds,
                "attempts": attempts,
                "elapsed_seconds": round(time.monotonic() - started, 3),
                **last_result,
            }
        time.sleep(5)
    raise EvalError(
        "docker",
        f"Neo4j readiness check failed for {case.container} after {timeout_seconds}s: {last_result}",
    )


def free_local_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind(("127.0.0.1", 0))
        return int(sock.getsockname()[1])


def ensure_neo4j(case: TestCase, cfg: dict[str, Any]) -> bool:
    running = docker_inspect_running(case.container)
    append_run_record(case, "docker_inspect", {"container": case.container, "running": running})
    if running is True:
        wait_result = wait_for_neo4j_ready(case)
        append_run_record(case, "docker_wait_ready", wait_result)
        return False
    if running is False:
        cmd = ["docker", "start", case.container]
        proc = run_process(cmd, ROOT)
        append_run_record(
            case,
            "docker_start",
            {"command": cmd, "returncode": proc.returncode, "stdout": proc.stdout, "stderr": proc.stderr},
        )
        if proc.returncode != 0:
            raise EvalError("docker", f"failed to start {case.container}: {proc.stderr.strip()}")
        wait_result = wait_for_neo4j_ready(case)
        append_run_record(case, "docker_wait_ready", wait_result)
        return False

    image = str(cfg.get("neo4j_image") or DEFAULT_NEO4J_IMAGE)
    docker_env = stringify_env(cfg.get("neo4j", {}) if isinstance(cfg.get("neo4j"), dict) else {})
    docker_env["NEO4J_AUTH"] = "none"
    cmd = [
        "docker",
        "run",
        "-d",
        "--name",
        case.container,
        "-p",
        f"{case.bolt_port}:7687",
        "-p",
        f"{case.http_port}:7474",
    ]
    for key, value in docker_env.items():
        cmd.extend(["-e", f"{key}={value}"])
    cmd.append(image)

    proc = run_process(cmd, ROOT)
    append_run_record(
        case,
        "docker_run",
        {
            "command": cmd,
            "returncode": proc.returncode,
            "stdout": proc.stdout,
            "stderr": proc.stderr,
            "image": image,
            "bolt_port": case.bolt_port,
            "http_port": case.http_port,
        },
    )
    if proc.returncode != 0:
        raise EvalError("docker", f"failed to create {case.container}: {proc.stderr.strip()}")
    wait_result = wait_for_neo4j_ready(case)
    append_run_record(case, "docker_wait_ready", wait_result)
    return True


def stop_neo4j(case: TestCase) -> dict[str, Any]:
    cmd = ["docker", "stop", "-t", "2", case.container]
    proc = run_cleanup_process(cmd, ROOT, timeout=10)
    return {
        "command": cmd,
        "returncode": proc.returncode,
        "stdout": proc.stdout,
        "stderr": proc.stderr,
    }


def remove_neo4j(case: TestCase) -> dict[str, Any]:
    cmd = ["docker", "rm", "-f", case.container]
    proc = run_cleanup_process(cmd, ROOT, timeout=10)
    return {
        "command": cmd,
        "returncode": proc.returncode,
        "stdout": proc.stdout,
        "stderr": proc.stderr,
    }


def write_database(case: TestCase, cfg: dict[str, Any]) -> None:
    build_dir = case.work / "build"
    if build_dir.exists():
        shutil.rmtree(build_dir)
    build_dir.mkdir()

    vast_env = stringify_env(cfg.get("vast", {}) if isinstance(cfg.get("vast"), dict) else {})

    configure = [
        "cmake",
        "-S",
        str(case.work / "bad"),
        "-B",
        "build",
        "-DCMAKE_C_COMPILER=wllvm",
        "-DCMAKE_CXX_COMPILER=wllvm++",
    ]
    configure_env = dict(vast_env)
    configure_env["WLLVM_CONFIGURE_ONLY"] = "1"
    configure_env["VAST_OUTPUT_FILE"] = str(case.work / "build" / "wllvm.log")
    proc, _ = run_database_process(case, "configure", configure, configure_env)
    if proc.returncode != 0:
        raise EvalError("db", f"cmake configure failed:\n{proc.stderr or proc.stdout}")

    build_env = dict(vast_env)
    build_env["VASTDB_NEO4J_ADDRESS"] = f"neo4j:@localhost:{case.bolt_port}"
    build_env["VAST_OUTPUT_FILE"] = str(case.work / "build" / "wllvm.log")
    proc, failure_reason = run_database_process(case, "build", ["cmake", "--build", "build"], build_env)
    if proc.returncode != 0:
        raise EvalError("db", f"cmake build/database write failed:\n{proc.stderr or proc.stdout}")
    if failure_reason:
        raise EvalError("db", f"cmake build/database write failed: {failure_reason}")


def run_database_process(
    case: TestCase,
    step: str,
    command: list[str],
    env: dict[str, str],
) -> tuple[subprocess.CompletedProcess[str], str | None]:
    base_record = {
        "step": step,
        "command": command,
        "cwd": str(case.work),
        "env": env,
    }
    try:
        proc = run_process(command, case.work, env=env)
    except BaseException as exc:
        append_run_record(
            case,
            f"write_database_{step}",
            {
                **base_record,
                "error_type": type(exc).__name__,
                "error": str(exc),
            },
        )
        raise
    failure_reason = database_failure_reason(step, env, proc)
    result_record = {
        **base_record,
        "returncode": proc.returncode,
        "stdout": proc.stdout,
        "stderr": proc.stderr,
    }
    if failure_reason:
        result_record["failure_reason"] = failure_reason
    append_run_record(
        case,
        f"write_database_{step}",
        result_record,
    )
    return proc, failure_reason


def database_failure_reason(
    step: str,
    env: dict[str, str],
    proc: subprocess.CompletedProcess[str],
) -> str | None:
    if step != "build" or proc.returncode != 0:
        return None
    output_file = env.get("VAST_OUTPUT_FILE")
    if not output_file:
        return None
    return vast_bitcode_failure_reason(output_file)


def vast_bitcode_failure_reason(path_value: str) -> str | None:
    try:
        for line in Path(path_value).read_text(encoding="utf-8", errors="replace").splitlines():
            if VAST_BITCODE_FAILURE_MARKER in line:
                return f"{path_value} contains '{VAST_BITCODE_FAILURE_MARKER}': {line}"
    except OSError:
        return None
    return None


def normalize_command(value: Any, default: list[str]) -> list[str]:
    if isinstance(value, str) and value:
        return [value]
    if isinstance(value, list) and all(isinstance(item, str) for item in value) and value:
        return value
    return default


def make_opencode_config(
    stage: str,
    case: TestCase,
) -> dict[str, Any]:
    config_path = OPENCODE_CONFIG_PATHS.get(stage)
    if config_path is None:
        raise EvalError("config", f"unknown opencode config stage: {stage}")
    config = read_json(config_path)
    if not isinstance(config.get("model"), str) or not config["model"]:
        raise EvalError("config", f"{config_path} must define a non-empty string field 'model'")

    if stage == "vastdb":
        mcp = config.setdefault("mcp", {})
        if not isinstance(mcp, dict):
            raise EvalError("config", f"{config_path} field 'mcp' must be an object")
        loc_mcp = mcp.setdefault("loc-mcp-server", {})
        if not isinstance(loc_mcp, dict):
            raise EvalError("config", f"{config_path} field 'mcp.loc-mcp-server' must be an object")
        mcp_env = stringify_env(loc_mcp.get("environment", {}) if isinstance(loc_mcp.get("environment"), dict) else {})
        mcp_env["NEO4J_URI"] = f"bolt://localhost:{case.bolt_port}"
        loc_mcp["environment"] = mcp_env
    return config


def parse_runner_stdout(stdout: str) -> dict[str, Any] | None:
    runner_result: dict[str, Any] | None = None
    for line in stdout.splitlines():
        stripped = line.strip()
        if not stripped:
            continue
        try:
            event = json.loads(stripped)
        except json.JSONDecodeError:
            continue
        if isinstance(event, dict) and ("ok" in event) and ("session_id" in event or "error" in event):
            runner_result = event
    return runner_result


def extract_assistant_text(runner_result: dict[str, Any]) -> str | None:
    return extract_latest_assistant_text(runner_result.get("response"))


def extract_latest_assistant_text(value: Any) -> str | None:
    candidates: list[str] = []

    def visit(item: Any, assistant_context: bool = False) -> None:
        if isinstance(item, dict):
            role = item.get("role") or item.get("type")
            info = item.get("info")
            if isinstance(info, dict):
                role = role or info.get("role") or info.get("type")
            is_assistant = assistant_context or str(role).lower() == "assistant"

            part_type = str(item.get("type") or "").lower()
            text = item.get("text")
            if is_assistant and part_type in ("text", "step_text", "message") and isinstance(text, str) and text.strip():
                candidates.append(text)

            content = item.get("content")
            if is_assistant and isinstance(content, str) and content.strip():
                candidates.append(content)

            for nested_key in ("data", "parts", "message", "messages", "response"):
                if nested_key in item:
                    visit(item[nested_key], is_assistant)
            for key, nested in item.items():
                if key not in {"data", "parts", "message", "messages", "response"}:
                    visit(nested, is_assistant)
        elif isinstance(item, list):
            for element in item:
                visit(element, assistant_context)

    visit(value)
    return candidates[-1] if candidates else None


def extract_structured_output(runner_result: dict[str, Any]) -> Any:
    return find_structured_tool_output(runner_result.get("response"))


def find_structured_tool_output(value: Any) -> Any:
    if isinstance(value, dict):
        if value.get("type") == "tool" and value.get("tool") == "StructuredOutput":
            state = value.get("state")
            if isinstance(state, dict) and state.get("status") == "completed" and "input" in state:
                return state["input"]
        for item in value.values():
            found = find_structured_tool_output(item)
            if found is not None:
                return found
    if isinstance(value, list):
        for item in value:
            found = find_structured_tool_output(item)
            if found is not None:
                return found
    return None


def extract_usage(runner_result: dict[str, Any]) -> dict[str, Any] | None:
    messages = runner_result.get("messages")
    if not isinstance(messages, dict):
        return None
    data = messages.get("data")
    if not isinstance(data, list):
        return None

    totals = {
        "tokens": {
            "total": 0,
            "input": 0,
            "output": 0,
        },
        "cost": 0,
    }
    saw_usage = False
    total_cost = 0
    has_cost = False
    for message in data:
        if not isinstance(message, dict):
            continue
        info = message.get("info")
        if not isinstance(info, dict):
            continue
        tokens = info.get("tokens")
        cost = info.get("cost")
        if isinstance(tokens, dict):
            for key in ("total", "input", "output"):
                value = tokens.get(key)
                if isinstance(value, (int, float)):
                    totals["tokens"][key] += value
                    saw_usage = True
        if isinstance(cost, (int, float)):
            total_cost += cost
            has_cost = True
            saw_usage = True

    if not saw_usage:
        return None
    totals["cost"] = total_cost if has_cost else None
    return totals


def run_agent(
    case: TestCase,
    stage: str,
    prompt: str,
    workdir: Path,
    result_dir: Path,
    output_name: str,
    format_config: dict[str, Any] | None = None,
) -> None:
    result_dir.mkdir(parents=True, exist_ok=True)

    opencode_config = make_opencode_config(stage, case)
    request = {
        "title": f"{case.name}-{stage}",
        "prompt": prompt,
        "agent": "build",
        "directory": str(workdir),
        "config": opencode_config,
        "port": free_local_port(),
        "timeout": 30000,
        "realtime": {
            "events": str(result_dir / "realtime_events.jsonl"),
            "text": str(result_dir / "realtime_output.txt"),
            "messages": str(result_dir / "realtime_messages.jsonl"),
        },
    }
    if format_config is not None:
        request["format"] = format_config
    request_file = result_dir / "opencode_request.json"
    write_json(request_file, request)

    cmd = ["node", str(OPENCODE_RUNNER), str(request_file)]

    started_at = dt.datetime.now(dt.timezone.utc)
    started = started_at.isoformat()
    started_monotonic = time.monotonic()
    proc = run_process(cmd, workdir)
    ended = dt.datetime.now(dt.timezone.utc).isoformat()
    duration = format_duration_seconds(time.monotonic() - started_monotonic)
    runner_result = parse_runner_stdout(proc.stdout)
    assistant_text = extract_assistant_text(runner_result) if runner_result else None
    usage = extract_usage(runner_result) if runner_result else None

    structured_output = extract_structured_output(runner_result) if runner_result and format_config else None
    if format_config is not None and structured_output is not None:
        final_output = json.dumps(structured_output, indent=2, ensure_ascii=False) + "\n"
    else:
        final_output = assistant_text or ""
    sdk_error = runner_error_message(runner_result) if runner_result else None

    output_path = result_dir / output_name
    output_path.write_text(final_output, encoding="utf-8")

    sdk_log: dict[str, Any] = {}
    if runner_result:
        sdk_log = {
            "ok": runner_result.get("ok"),
            "server_url": runner_result.get("server_url"),
            "session_id": runner_result.get("session_id"),
            "config_model": runner_result.get("config_model"),
            "prompt_agent": runner_result.get("prompt_agent"),
            "requested_config": runner_result.get("requested_config"),
            "resolved_config": runner_result.get("resolved_config"),
            "mcp_status": runner_result.get("mcp_status"),
            "tool_listing": runner_result.get("tool_listing"),
            "skills": runner_result.get("skills"),
            "realtime": runner_result.get("realtime"),
            "response": runner_result.get("response"),
            "messages": runner_result.get("messages"),
            "error": runner_result.get("error"),
        }

    log = {
        "stage": stage,
        "case": case.name,
        "started_at": started,
        "ended_at": ended,
        "duration": duration,
        "workdir": str(workdir),
        "process": {
            "command": mask_command(cmd),
            "returncode": proc.returncode,
            "stderr": proc.stderr,
            "stdout_raw": proc.stdout if runner_result is None else None,
        },
        "sdk": sdk_log,
        "usage": usage,
        "output": {
            "file": str(output_path),
            "content": final_output,
        },
        "error": None,
    }
    if proc.returncode != 0:
        log["error"] = f"opencode failed with exit code {proc.returncode}"
        write_json(result_dir / "log.json", log)
        raise EvalError(stage, log["error"])
    if sdk_error:
        log["error"] = sdk_error
        write_json(result_dir / "log.json", log)
        raise EvalError(stage, log["error"])
    if not final_output.strip():
        log["error"] = f"{stage} did not produce {output_name}"
        write_json(result_dir / "log.json", log)
        raise EvalError(stage, log["error"])

    write_json(result_dir / "log.json", log)


def runner_error_message(runner_result: dict[str, Any] | None) -> str | None:
    if not runner_result:
        return None
    if runner_result.get("ok") is False:
        error = runner_result.get("error")
        if error:
            return f"opencode runner failed: {compact_json(error)}"
        return "opencode runner failed"
    response = runner_result.get("response")
    if isinstance(response, dict) and "error" in response:
        return f"opencode session.prompt returned error: {compact_json(response.get('error'))}"
    return None


def compact_json(value: Any) -> str:
    if isinstance(value, str):
        return value
    try:
        return json.dumps(value, ensure_ascii=False, sort_keys=True)
    except TypeError:
        return str(value)


def mask_command(cmd: list[str]) -> list[str]:
    if not cmd:
        return cmd
    masked = list(cmd)
    if len(masked[-1]) > 300:
        masked[-1] = f"<prompt omitted: {len(masked[-1])} chars>"
    return masked


def validate_judge_output(result_dir: Path) -> None:
    output_path = result_dir / "output.json"
    raw = output_path.read_text(encoding="utf-8")
    try:
        data = parse_json_lenient(raw)
    except json.JSONDecodeError as exc:
        raise EvalError("judge", f"judge output is not valid JSON: {exc}") from exc
    if raw != json.dumps(data, indent=2, ensure_ascii=False) + "\n":
        output_path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    for key in ("baseline", "vastdb"):
        entry = data.get(key)
        if not isinstance(entry, dict):
            raise EvalError("judge", f"judge output missing object: {key}")
        if not isinstance(entry.get("correct"), bool):
            raise EvalError("judge", f"judge output {key}.correct must be boolean")
        score = entry.get("score")
        if not isinstance(score, (int, float)) or score < 0 or score > 10:
            raise EvalError("judge", f"judge output {key}.score must be 0-10")
        if not isinstance(entry.get("reason"), str):
            raise EvalError("judge", f"judge output {key}.reason must be string")


def parse_json_lenient(text: str) -> Any:
    stripped = text.strip()
    fenced = re.fullmatch(r"```(?:json|JSON)?\s*(.*?)\s*```", stripped, flags=re.DOTALL)
    if fenced:
        stripped = fenced.group(1).strip()
    try:
        return json.loads(stripped)
    except json.JSONDecodeError:
        start = stripped.find("{")
        end = stripped.rfind("}")
        if start == -1 or end == -1 or end <= start:
            raise
        return json.loads(stripped[start : end + 1])


def build_score_statistics(
    results: list[dict[str, Any]],
    metrics_by_case: dict[str, dict[str, Any]],
) -> dict[str, Any]:
    summary: dict[str, Any] = {
        "evaluated": 0,
        "skipped": 0,
        "baseline": {
            "correct": 0,
            "avg_score": None,
        },
        "vastdb": {
            "correct": 0,
            "avg_score": None,
        },
        "vastdb_better": 0,
    }
    baseline_score_total = 0.0
    vastdb_score_total = 0.0

    for result in results:
        if not result.get("ok"):
            summary["skipped"] += 1
            continue
        case_name = result.get("case")
        if not isinstance(case_name, str):
            summary["skipped"] += 1
            continue

        metrics = metrics_by_case.get(case_name)
        if not metrics:
            summary["skipped"] += 1
            continue
        score = metrics.get("score")
        if not isinstance(score, dict):
            summary["skipped"] += 1
            continue

        baseline = score["baseline"]
        vastdb = score["vastdb"]
        baseline_score = float(baseline["score"])
        vastdb_score = float(vastdb["score"])
        summary["evaluated"] += 1
        baseline_score_total += baseline_score
        vastdb_score_total += vastdb_score
        if baseline.get("correct") is True:
            summary["baseline"]["correct"] += 1
        if vastdb.get("correct") is True:
            summary["vastdb"]["correct"] += 1
        if vastdb_score >= baseline_score:
            summary["vastdb_better"] += 1

    evaluated = summary["evaluated"]
    if evaluated:
        summary["baseline"]["avg_score"] = round(baseline_score_total / evaluated, 2)
        summary["vastdb"]["avg_score"] = round(vastdb_score_total / evaluated, 2)
    return summary


def read_judge_output(case_name: str) -> dict[str, Any]:
    output_path = case_work_dir(case_name) / "results" / "judge" / "output.json"
    data = read_json(output_path)
    for key in ("baseline", "vastdb"):
        entry = data.get(key)
        if not isinstance(entry, dict):
            raise EvalError("summary", f"judge output missing object: {key}")
        if not isinstance(entry.get("correct"), bool):
            raise EvalError("summary", f"judge output {key}.correct must be boolean")
        score = entry.get("score")
        if not isinstance(score, (int, float)):
            raise EvalError("summary", f"judge output {key}.score must be number")
    return data


def case_work_dir(case_name: str) -> Path:
    match = re.fullmatch(r"CWD-(\d+)-(\d+)", case_name)
    if not match:
        raise EvalError("summary", f"invalid case name: {case_name}")
    return OUTPUTS_DIR / f"CWD-{match.group(1)}" / case_name


def build_case_metrics(result: dict[str, Any]) -> dict[str, Any]:
    case_name = result.get("case")
    metrics: dict[str, Any] = {
        "case": case_name,
        "score": None,
        "score_error": None,
        "durations": {},
    }
    if not isinstance(case_name, str):
        metrics["score_error"] = "missing case name"
        return metrics

    durations: dict[str, float] = {}
    for stage in ("baseline", "vastdb", "judge"):
        try:
            durations[stage] = read_stage_duration_seconds(case_name, stage)
        except EvalError:
            continue
    metrics["durations"] = durations

    if result.get("ok") is not True:
        return metrics

    try:
        judge = read_judge_output(case_name)
        metrics["score"] = {
            "baseline": {
                "correct": judge["baseline"]["correct"],
                "score": float(judge["baseline"]["score"]),
            },
            "vastdb": {
                "correct": judge["vastdb"]["correct"],
                "score": float(judge["vastdb"]["score"]),
            },
        }
    except (EvalError, KeyError, TypeError, ValueError) as exc:
        metrics["score_error"] = str(exc)
    return metrics


def build_metrics_cache(results: list[dict[str, Any]]) -> dict[str, dict[str, Any]]:
    cache: dict[str, dict[str, Any]] = {}
    for result in results:
        case_name = result.get("case")
        if isinstance(case_name, str):
            cache[case_name] = build_case_metrics(result)
    return cache


def build_average_duration_statistics(
    results: list[dict[str, Any]],
    metrics_by_case: dict[str, dict[str, Any]],
) -> dict[str, Any]:
    summary: dict[str, Any] = {}
    for stage in ("baseline", "vastdb", "judge"):
        durations: list[float] = []
        for result in results:
            case_name = result.get("case")
            if not isinstance(case_name, str):
                continue
            metrics = metrics_by_case.get(case_name)
            if not metrics:
                continue
            stage_durations = metrics.get("durations")
            if not isinstance(stage_durations, dict):
                continue
            duration = stage_durations.get(stage)
            if isinstance(duration, (int, float)):
                durations.append(float(duration))
        if durations:
            average = sum(durations) / len(durations)
            summary[stage] = format_duration_seconds(average)
        else:
            summary[stage] = None
    return summary


def read_stage_duration_seconds(case_name: str, stage: str) -> float:
    log_path = case_work_dir(case_name) / "results" / stage / "log.json"
    data = read_json(log_path)
    duration = data.get("duration")
    if isinstance(duration, str):
        parsed = parse_duration_seconds(duration)
        if parsed is not None:
            return float(parsed)
    started = data.get("started_at")
    ended = data.get("ended_at")
    if isinstance(started, str) and isinstance(ended, str):
        try:
            started_at = dt.datetime.fromisoformat(started)
            ended_at = dt.datetime.fromisoformat(ended)
        except ValueError as exc:
            raise EvalError("summary", f"invalid duration timestamps in {log_path}") from exc
        return max(0.0, (ended_at - started_at).total_seconds())
    raise EvalError("summary", f"missing duration in {log_path}")


def parse_duration_seconds(value: str) -> int | None:
    match = re.fullmatch(r"(\d+)m(\d{1,2})s", value)
    if not match:
        return None
    return int(match.group(1)) * 60 + int(match.group(2))


def build_statistics(
    results: list[dict[str, Any]],
    metrics_by_case: dict[str, dict[str, Any]],
) -> dict[str, Any]:
    return {
        "score": build_score_statistics(results, metrics_by_case),
        "avg_duration": build_average_duration_statistics(results, metrics_by_case),
    }


def case_name_sort_key(name: Any) -> tuple[int, int, str]:
    if isinstance(name, str):
        match = re.fullmatch(r"CWD-(\d+)-(\d+)", name)
        if match:
            return (int(match.group(1)), int(match.group(2)), name)
        return (sys.maxsize, sys.maxsize, name)
    return (sys.maxsize, sys.maxsize, str(name))


def sorted_results(results: list[dict[str, Any]]) -> list[dict[str, Any]]:
    return sorted(results, key=lambda item: case_name_sort_key(item.get("case")))


def result_statistics(metrics: dict[str, Any] | None) -> dict[str, Any]:
    if not metrics:
        return {
            "score": None,
            "score_error": "missing in-memory metrics",
            "duration": {},
        }
    score = metrics.get("score")
    durations = metrics.get("durations")
    formatted_durations: dict[str, str] = {}
    if isinstance(durations, dict):
        for stage in ("baseline", "vastdb", "judge"):
            duration = durations.get(stage)
            if isinstance(duration, (int, float)):
                formatted_durations[stage] = format_duration_seconds(float(duration))

    statistics: dict[str, Any] = {
        "score": score if isinstance(score, dict) else None,
        "duration": formatted_durations,
    }
    if statistics["score"] is None:
        statistics["score_error"] = str(metrics.get("score_error") or "missing judge metrics")
    return statistics


def results_with_statistics(
    results: list[dict[str, Any]],
    metrics_by_case: dict[str, dict[str, Any]],
) -> list[dict[str, Any]]:
    enriched: list[dict[str, Any]] = []
    for result in sorted_results(results):
        item = dict(result)
        case_name = item.get("case")
        item["statistics"] = result_statistics(metrics_by_case.get(case_name) if isinstance(case_name, str) else None)
        enriched.append(item)
    return enriched


def results_by_case(results: list[dict[str, Any]]) -> dict[str, dict[str, Any]]:
    indexed: dict[str, dict[str, Any]] = {}
    for result in results:
        case_name = result.get("case")
        if isinstance(case_name, str):
            indexed[case_name] = result
    return indexed


def load_summary_results(path: Path) -> dict[str, dict[str, Any]]:
    if not path.exists():
        return {}
    try:
        summary = read_json(path)
    except EvalError:
        return {}
    results = summary.get("results")
    if not isinstance(results, list):
        return {}
    return results_by_case([item for item in results if isinstance(item, dict)])


def build_summary_document(
    run_time: str,
    jobs: int,
    total: int,
    pending: int,
    results: list[dict[str, Any]],
    metrics_by_case: dict[str, dict[str, Any]],
) -> dict[str, Any]:
    ordered_results = sorted_results(results)
    return {
        "time": run_time,
        "jobs": jobs,
        "total": total,
        "pending": pending,
        "ok": sum(1 for item in ordered_results if item.get("ok") is True),
        "failed": sum(1 for item in ordered_results if item.get("ok") is not True),
        "statistics": build_statistics(ordered_results, metrics_by_case),
        "results": results_with_statistics(ordered_results, metrics_by_case),
    }


def write_summaries(
    run_time: str,
    jobs: int,
    summary_total: int,
    current_total: int,
    pending: int,
    cumulative_results: dict[str, dict[str, Any]],
    current_results: dict[str, dict[str, Any]],
    cumulative_metrics: dict[str, dict[str, Any]],
    current_metrics: dict[str, dict[str, Any]],
) -> None:
    write_json(
        SUMMARY_PATH,
        build_summary_document(
            run_time,
            jobs,
            summary_total,
            pending,
            list(cumulative_results.values()),
            cumulative_metrics,
        ),
    )
    write_json(
        CUR_SUMMARY_PATH,
        build_summary_document(
            run_time,
            jobs,
            current_total,
            pending,
            list(current_results.values()),
            current_metrics,
        ),
    )


def append_run_record(case: TestCase, kind: str, data: dict[str, Any]) -> None:
    run_path = case.work / "results" / "run" / "run.json"
    record = {
        "time": now_east8_iso(),
        "case": case.name,
        "kind": kind,
        **data,
    }
    if run_path.exists():
        try:
            run_log = read_json(run_path)
        except EvalError:
            run_log = {"case": case.name, "events": []}
    else:
        run_log = {"case": case.name, "events": []}
    events = run_log.setdefault("events", [])
    if not isinstance(events, list):
        events = []
        run_log["events"] = events
    events.append(record)
    write_json(run_path, run_log)


def record_failure(case: TestCase, stage: str, message: str) -> None:
    if stage in ("baseline", "vastdb", "judge"):
        stage_dir = case.work / "results" / stage
        stage_dir.mkdir(parents=True, exist_ok=True)
        log_path = stage_dir / "log.json"
        if log_path.exists():
            try:
                log = read_json(log_path)
            except EvalError:
                log = {"stage": stage, "case": case.name}
            log["validation_error"] = {
                "time": now_east8_iso(),
                "error": message,
            }
            write_json(log_path, log)
        else:
            write_json(
                log_path,
                {
                    "stage": stage,
                    "case": case.name,
                    "time": now_east8_iso(),
                    "error": message,
                },
            )
    else:
        append_run_record(case, "failure", {"stage": stage, "error": message})


def run_case(case: TestCase, cfg: dict[str, Any]) -> dict[str, Any]:
    created_container = False
    container_removed = False
    status = {"case": case.name, "ok": False, "stage": None, "error": None}
    try:
        copy_case_to_work(case)
        cwe_id, location_text = parse_cve(case)
        created_container = run_progress_stage(case, "load database", lambda: ensure_neo4j(case, cfg))
        if created_container:
            try:
                run_progress_stage(case, "write database", lambda: write_database(case, cfg))
            except BaseException:
                remove_result = remove_neo4j(case)
                container_removed = True
                append_run_record(case, "docker_rm", {"reason": "incomplete_database", **remove_result})
                raise
        else:
            append_run_record(case, "write_database_skip", {"reason": "existing_container", "container": case.container})

        results = case.work / "results"

        baseline_prompt = render_template(BASELINE_TEMPLATE, cwe_id=cwe_id, location_text=location_text)
        run_progress_stage(
            case,
            "baseline agent",
            lambda: run_agent(
                case,
                "baseline",
                baseline_prompt,
                case.work / "bad",
                results / "baseline",
                "output.txt",
            ),
        )

        vastdb_prompt = render_template(VASTDB_TEMPLATE, cwe_id=cwe_id, location_text=location_text)
        run_progress_stage(
            case,
            "vastdb agent",
            lambda: run_agent(
                case,
                "vastdb",
                vastdb_prompt,
                case.work / "bad",
                results / "vastdb",
                "output.txt",
            ),
        )

        judge_prompt = render_template(
            JUDGE_TEMPLATE,
            baseline_output="results/baseline/output.txt",
            vastdb_output="results/vastdb/output.txt",
        )
        judge_format = read_json(JUDGE_SCHEMA)
        run_progress_stage(
            case,
            "judge agent",
            lambda: run_agent(
                case,
                "judge",
                judge_prompt,
                case.work,
                results / "judge",
                "output.json",
                judge_format,
            ),
        )
        validate_judge_output(results / "judge")
        status["ok"] = True
        return status
    except EvalError as exc:
        status.update({"stage": exc.stage, "error": exc.message})
        record_failure(case, exc.stage, exc.message)
        return status
    except Exception as exc:
        status.update({"stage": "unexpected", "error": repr(exc)})
        record_failure(case, "unexpected", repr(exc))
        return status
    finally:
        if case.work.exists() and not container_removed:
            stop_result = stop_neo4j(case)
            append_run_record(case, "docker_stop", stop_result)


def main() -> int:
    try:
        args = parse_args()
        if args.jobs < 1:
            raise EvalError("input", "--jobs must be >= 1")
        cfg = load_env_config()
        cases = select_cases(args)
        check_runtime_dependencies()
    except EvalError as exc:
        print(f"[{exc.stage}] {exc.message}", file=sys.stderr)
        return 2

    if not cases:
        print("No test cases selected.")
        return 0

    reset_progress_log()
    print(f"Selected {len(cases)} test case(s), jobs={args.jobs}")
    run_time = now_east8_iso()
    cumulative_results = load_summary_results(SUMMARY_PATH)
    cumulative_metrics = build_metrics_cache(list(cumulative_results.values()))
    current_results: dict[str, dict[str, Any]] = {}
    current_metrics: dict[str, dict[str, Any]] = {}
    pending_cases = {case.name for case in cases}
    summary_total = len(set(cumulative_results) | pending_cases)
    current_total = len(cases)

    def record_result(result: dict[str, Any]) -> None:
        case_name = result.get("case")
        if isinstance(case_name, str):
            cumulative_results[case_name] = result
            current_results[case_name] = result
            metrics = build_case_metrics(result)
            cumulative_metrics[case_name] = metrics
            current_metrics[case_name] = metrics
            pending_cases.discard(case_name)
        write_summaries(
            run_time,
            args.jobs,
            summary_total,
            current_total,
            len(pending_cases),
            cumulative_results,
            current_results,
            cumulative_metrics,
            current_metrics,
        )

    write_summaries(
        run_time,
        args.jobs,
        summary_total,
        current_total,
        len(pending_cases),
        cumulative_results,
        current_results,
        cumulative_metrics,
        current_metrics,
    )

    try:
        if args.jobs == 1:
            for case in cases:
                print(f"Running {case.name}")
                result = run_case(case, cfg)
                record_result(result)
                print_result(result)
        else:
            with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as executor:
                future_to_case = {executor.submit(run_case, case, cfg): case for case in cases}
                for future in concurrent.futures.as_completed(future_to_case):
                    result = future.result()
                    record_result(result)
                    print_result(result)
    except KeyboardInterrupt:
        print("Interrupted. Cleanup completed for the active test case where possible.", file=sys.stderr)
        return 130

    return 0 if all(item.get("ok") is True for item in current_results.values()) else 1


def print_result(result: dict[str, Any]) -> None:
    if result["ok"]:
        print(f"OK {result['case']}")
    else:
        print(f"FAIL {result['case']} [{result['stage']}]: {result['error']}", file=sys.stderr)


if __name__ == "__main__":
    raise SystemExit(main())
