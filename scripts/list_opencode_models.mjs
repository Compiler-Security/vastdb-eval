#!/usr/bin/env node

import fs from "node:fs/promises"
import path from "node:path"
import { createOpencode } from "@opencode-ai/sdk/v2"

const ROOT = path.resolve(path.dirname(new URL(import.meta.url).pathname), "..")
const DEFAULT_OUTPUT = path.join(ROOT, "outputs", "models.json")

async function main() {
  const outputPath = path.resolve(process.argv[2] ?? DEFAULT_OUTPUT)
  const startedAt = new Date().toISOString()
  const port = 59000 + (process.pid % 1000)

  const opencode = await createOpencode({
    hostname: "127.0.0.1",
    port,
    config: {},
    timeout: 30000,
  })

  try {
    const directory = ROOT
    const result = await opencode.client.provider.list({ directory })
    const data = result.data ?? {}
    const providers = Array.isArray(data.all) ? data.all : []

    const output = {
      generated_at: new Date().toISOString(),
      started_at: startedAt,
      directory,
      server_url: opencode.server.url,
      provider_count: providers.length,
      model_count: providers.reduce((sum, provider) => sum + Object.keys(provider.models).length, 0),
      default: data.default ?? null,
      connected: data.connected ?? null,
      providers,
      response: result.response ?? null,
    }

    await fs.mkdir(path.dirname(outputPath), { recursive: true })
    await fs.writeFile(outputPath, `${JSON.stringify(output, null, 2)}\n`)
    console.log(outputPath)
  } finally {
    opencode.server.close()
  }
}

main().catch((error) => {
  console.error(error?.stack ?? error?.message ?? String(error))
  process.exit(1)
})
