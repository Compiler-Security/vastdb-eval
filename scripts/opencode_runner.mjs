#!/usr/bin/env node

import fs from "node:fs/promises"
import { createOpencode } from "@opencode-ai/sdk/v2"

async function readRequest() {
  const requestPath = process.argv[2]
  if (!requestPath) {
    throw new Error("usage: node scripts/opencode_runner.mjs <request.json>")
  }
  return JSON.parse(await fs.readFile(requestPath, "utf8"))
}

function parseModel(model) {
  if (!model || typeof model !== "string" || !model.includes("/")) {
    return undefined
  }
  const [providerID, ...rest] = model.split("/")
  const modelID = rest.join("/")
  if (!providerID || !modelID) {
    return undefined
  }
  return { providerID, modelID }
}

async function collectToolListing(client, model, directory) {
  try {
    const ids = await client.tool.ids({
      ...(directory ? { directory } : {}),
    })
    const listing = model
      ? await client.tool.list({
          ...(directory ? { directory } : {}),
          provider: model.providerID,
          model: model.modelID,
        })
      : null
    return {
      ok: true,
      ids,
      list: listing,
    }
  } catch (error) {
    return {
      ok: false,
      error: {
        name: error?.name,
        message: error?.message ?? String(error),
        stack: error?.stack,
      },
    }
  }
}

async function collectSkills(opencode, directory) {
  try {
    return await opencode.client.app.skills({
      ...(directory ? { directory } : {}),
    })
  } catch (error) {
    return {
      ok: false,
      error: {
        name: error?.name,
        message: error?.message ?? String(error),
        stack: error?.stack,
      },
    }
  }
}

async function main() {
  const request = await readRequest()
  const startedAt = new Date().toISOString()
  const configModel = typeof request.config?.model === "string" ? request.config.model : undefined
  const listingModel = parseModel(configModel)
  const directory = request.directory
  const opencode = await createOpencode({
    hostname: "127.0.0.1",
    port: request.port ?? 0,
    config: request.config ?? {},
    timeout: request.timeout ?? 30000,
  })

  try {
    const query = directory ? { directory } : {}
    const resolvedConfig = await opencode.client.config.get(query)
    const mcpStatus = await opencode.client.mcp.status(query)

    const session = await opencode.client.session.create({
      ...query,
      title: request.title ?? "vastdb-eval",
    })
    const sessionId = session.data?.id ?? session.id
    if (!sessionId) {
      throw new Error("opencode SDK did not return a session id")
    }

    const toolListing = await collectToolListing(opencode.client, listingModel, directory)
    const skills = await collectSkills(opencode, directory)

    const body = {
      parts: [{ type: "text", text: request.prompt }],
    }
    if (request.agent) {
      body.agent = request.agent
    }
    if (request.format) {
      body.format = request.format
    }

    const response = await opencode.client.session.prompt({
      ...query,
      sessionID: sessionId,
      ...body,
    })
    const messages = await opencode.client.session.messages({
      ...query,
      sessionID: sessionId,
    })

    console.log(JSON.stringify({
      ok: true,
      started_at: startedAt,
      ended_at: new Date().toISOString(),
      server_url: opencode.server.url,
      session_id: sessionId,
      config_model: configModel ?? null,
      prompt_agent: request.agent ?? null,
      requested_config: request.config ?? {},
      resolved_config: resolvedConfig,
      mcp_status: mcpStatus,
      tool_listing: toolListing,
      skills,
      response,
      messages,
    }))
  } finally {
    opencode.server.close()
  }
}

main().catch((error) => {
  console.log(JSON.stringify({
    ok: false,
    ended_at: new Date().toISOString(),
    error: {
      name: error?.name,
      message: error?.message ?? String(error),
      stack: error?.stack,
    },
  }))
  process.exit(1)
})
