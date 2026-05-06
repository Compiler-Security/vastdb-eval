#!/usr/bin/env python3
"""Copy outputs and keep only compact per-testcase result artifacts."""

from __future__ import annotations

import argparse
import json
import shutil
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "outputs"
DEFAULT_DEST = ROOT / "outputs-export"
REMOVE_RESULT_RELATIVE_PATHS = (Path("run"),)
KEEP_TESTCASE_ENTRY_NAMES = {"results"}
KEEP_RESULT_FILENAMES = {"output.txt"}
KEEP_RESULT_RELATIVE_PATHS = {Path("judge/output.json")}
TOP_LEVEL_KEEP_FILENAMES = {"summary.json"}
REDACTED_VALUE = "<redacted>"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Copy vastdb-eval outputs to a new directory, then keep only "
            "results/ under every CWD testcase directory. Remaining results "
            "subdirectories keep only output.txt and results/judge/output.json "
            "by default. "
            "Top-level output files keep only summary.json."
        )
    )
    parser.add_argument(
        "--source",
        type=Path,
        default=DEFAULT_SOURCE,
        help=f"source outputs directory (default: {DEFAULT_SOURCE})",
    )
    parser.add_argument(
        "--dest",
        type=Path,
        default=DEFAULT_DEST,
        help=f"destination directory for the pruned copy (default: {DEFAULT_DEST})",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="show what would be removed without copying or deleting anything",
    )
    parser.add_argument(
        "--full",
        action="store_true",
        help="export a fuller copy with more diagnostic artifacts, including results/**/log.json",
    )
    return parser.parse_args()


def is_relative_to(path: Path, parent: Path) -> bool:
    try:
        path.relative_to(parent)
    except ValueError:
        return False
    return True


def is_testcase_dir(path: Path) -> bool:
    parts = path.name.split("-")
    if len(parts) != 3 or parts[0] != "CWD":
        return False
    expected_parent = "-".join(parts[:2])
    return path.parent.name == expected_parent


def testcase_dirs(root: Path) -> list[Path]:
    return sorted(
        path for path in root.rglob("CWD-*") if path.is_dir() and is_testcase_dir(path)
    )


def validate_force_dest(source: Path, dest: Path) -> None:
    protected = {
        Path("/").resolve(),
        Path.home().resolve(),
        ROOT,
        ROOT.parent,
        source,
        source.parent,
    }
    if dest in protected:
        raise SystemExit(f"refusing to replace protected directory: {dest}")


def remove_path(path: Path) -> bool:
    if not path.exists() and not path.is_symlink():
        return False

    if path.is_dir() and not path.is_symlink():
        shutil.rmtree(path)
    else:
        path.unlink()
    return True


def top_level_removals(root: Path) -> list[Path]:
    return sorted(
        path
        for path in root.iterdir()
        if not (path.name in TOP_LEVEL_KEEP_FILENAMES or path.name.startswith("CWD-"))
    )


def plan_removals(root: Path, full: bool = False) -> tuple[list[Path], int]:
    cases = testcase_dirs(root)
    removals: list[Path] = top_level_removals(root)
    for case in cases:
        for path in sorted(case.iterdir()):
            if path.name not in KEEP_TESTCASE_ENTRY_NAMES:
                removals.append(path)

        results_dir = case / "results"
        if results_dir.is_dir():
            for relative_path in REMOVE_RESULT_RELATIVE_PATHS:
                path = results_dir / relative_path
                if path.exists() or path.is_symlink():
                    removals.append(path)

            for path in sorted(results_dir.rglob("*")):
                if any(parent in removals for parent in path.parents):
                    continue
                if path.is_file() or path.is_symlink():
                    relative_path = path.relative_to(results_dir)
                    if path.name == "log.json" and not full:
                        removals.append(path)
                        continue
                    if path.name == "log.json" and full:
                        continue
                    if (
                        path.name not in KEEP_RESULT_FILENAMES
                        and relative_path not in KEEP_RESULT_RELATIVE_PATHS
                    ):
                        removals.append(path)
    return removals, len(cases)


def copy_outputs(source: Path, dest: Path) -> None:
    if dest.exists():
        validate_force_dest(source, dest)
        shutil.rmtree(dest)

    shutil.copytree(source, dest, symlinks=True)


def redact_provider_options_api_keys(value: object) -> int:
    redacted = 0
    if isinstance(value, dict):
        options = value.get("options")
        if isinstance(options, dict) and "apiKey" in options:
            options["apiKey"] = REDACTED_VALUE
            redacted += 1
        for child in value.values():
            redacted += redact_provider_options_api_keys(child)
    elif isinstance(value, list):
        for child in value:
            redacted += redact_provider_options_api_keys(child)
    return redacted


def redact_exported_log(path: Path) -> int:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return 0
    if not isinstance(data, dict):
        return 0

    sdk = data.get("sdk")
    if not isinstance(sdk, dict):
        return 0

    redacted = 0
    for key in ("requested_config", "resolved_config"):
        config = sdk.get(key)
        if isinstance(config, dict):
            provider = config.get("provider")
            if provider is not None:
                redacted += redact_provider_options_api_keys(provider)

    if redacted:
        path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    return redacted


def redact_exported_logs(root: Path) -> tuple[int, int]:
    log_count = 0
    redacted_count = 0
    for path in sorted(root.glob("CWD-*/CWD-*/results/**/log.json")):
        log_count += 1
        redacted_count += redact_exported_log(path)
    return log_count, redacted_count


def main() -> int:
    args = parse_args()
    source = args.source.expanduser().resolve()
    dest = args.dest.expanduser().resolve()

    if not source.is_dir():
        raise SystemExit(f"source is not a directory: {source}")
    if source == dest:
        raise SystemExit("destination must be different from source")
    if is_relative_to(dest, source):
        raise SystemExit("destination must not be inside source")

    root_for_plan = source if args.dry_run else dest
    if args.dry_run:
        removals, case_count = plan_removals(source, args.full)
        print(f"Would copy: {source} -> {dest}")
        print(f"Found {case_count} testcase directories.")
        print(f"Would remove {len(removals)} paths from the copy.")
        for path in removals:
            print(path.relative_to(source))
        return 0

    copy_outputs(source, dest)
    removals, case_count = plan_removals(root_for_plan, args.full)
    removed = 0
    for path in removals:
        removed += int(remove_path(path))
    log_count, redacted_count = redact_exported_logs(dest)

    print(f"Copied: {source} -> {dest}")
    print(f"Found {case_count} testcase directories.")
    print(f"Removed {removed} paths from the copy.")
    print(f"Redacted provider options apiKey in {redacted_count} config object(s) across {log_count} log files.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
