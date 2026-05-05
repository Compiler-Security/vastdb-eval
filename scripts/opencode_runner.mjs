#!/usr/bin/env node

import { createWriteStream } from "node:fs"
import fs from "node:fs/promises"
import { Agent } from "undici"
import { createOpencodeClient, createOpencodeServer } from "@opencode-ai/sdk/v2"

const fetchDispatcher = new Agent({
  headersTimeout: 0,
  bodyTimeout: 0,
})

function fetchWithoutUndiciTimeout(input, init = {}) {
  return fetch(input, {
    ...init,
    dispatcher: fetchDispatcher,
  })
}

async function createEvalOpencode(options) {
  const server = await createOpencodeServer({
    ...options,
  })
  const client = createOpencodeClient({
    baseUrl: server.url,
    fetch: fetchWithoutUndiciTimeout,
  })
  return {
    client,
    server,
  }
}

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

function eventSessionID(event) {
  const properties = event?.properties
  if (typeof properties?.sessionID === "string") {
    return properties.sessionID
  }
  return undefined
}

function eventTextDelta(event) {
  if (event?.type !== "message.part.delta") {
    return ""
  }
  const properties = event.properties ?? {}
  if (properties.field !== "text" || typeof properties.delta !== "string") {
    return ""
  }
  return properties.delta
}

function errorInfo(error, event) {
  return {
    name: error?.name,
    message: error?.message ?? String(error),
    stack: error?.stack,
    event_type: event?.type ?? null,
  }
}

function clone(value) {
  return JSON.parse(JSON.stringify(value))
}

function createMessageState(messagesStream, errors) {
  const emittedParts = new Set()

  function partFinished(part) {
    if (!part || typeof part !== "object") {
      return false
    }
    if (part.type === "step-start" || part.type === "step-finish") {
      return true
    }
    if (part.time?.end !== undefined) {
      return true
    }
    if (part.state?.status === "completed" || part.state?.status === "failed") {
      return true
    }
    return false
  }

  function writePart(part) {
    const partID = part?.id
    if (!messagesStream || !partID || emittedParts.has(partID)) {
      return
    }
    emittedParts.add(partID)
    messagesStream.write(`${JSON.stringify(clone(part))}\n`)
  }

  return {
    apply(event) {
      try {
        const properties = event?.properties ?? {}
        if (event?.type === "message.part.updated") {
          const part = properties.part
          if (partFinished(part)) {
            writePart(part)
          }
        }
      } catch (error) {
        errors.push(errorInfo(error, event))
      }
    },
  }
}

async function startEventRecorder(client, query, sessionId, realtime) {
  if (!realtime?.events && !realtime?.text && !realtime?.messages) {
    return {
      stop: async () => {},
      info: null,
    }
  }

  const controller = new AbortController()
  const eventsStream = realtime.events
    ? createWriteStream(realtime.events, { flags: "w", encoding: "utf8" })
    : null
  const textStream = realtime.text
    ? createWriteStream(realtime.text, { flags: "w", encoding: "utf8" })
    : null
  const messagesStream = realtime.messages
    ? createWriteStream(realtime.messages, { flags: "w", encoding: "utf8" })
    : null
  const errors = []
  const messageState = createMessageState(messagesStream, errors)

  const recording = (async () => {
    try {
      const events = await client.event.subscribe(query, { signal: controller.signal })
      for await (const event of events.stream) {
        const currentSessionID = eventSessionID(event)
        if (currentSessionID && currentSessionID !== sessionId) {
          continue
        }
        eventsStream?.write(`${JSON.stringify({
          recorded_at: new Date().toISOString(),
          event,
        })}\n`)
        messageState.apply(event)
        const delta = eventTextDelta(event)
        if (delta) {
          textStream?.write(delta)
        }
      }
    } catch (error) {
      if (!controller.signal.aborted) {
        errors.push(errorInfo(error))
      }
    }
  })()

  return {
    info: {
      events_file: realtime.events ?? null,
      text_file: realtime.text ?? null,
      messages_file: realtime.messages ?? null,
      errors,
    },
    stop: async () => {
      controller.abort()
      const timeout = new Promise((resolve) => setTimeout(resolve, 5000))
      await Promise.race([recording, timeout])
      await Promise.all([
        eventsStream ? new Promise((resolve) => eventsStream.end(resolve)) : null,
        textStream ? new Promise((resolve) => textStream.end(resolve)) : null,
        messagesStream ? new Promise((resolve) => messagesStream.end(resolve)) : null,
      ].filter(Boolean))
    },
  }
}

async function main() {
  const request = await readRequest()
  const startedAt = new Date().toISOString()
  const configModel = typeof request.config?.model === "string" ? request.config.model : undefined
  const listingModel = parseModel(configModel)
  const directory = request.directory
  const opencode = await createEvalOpencode({
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
    const eventRecorder = await startEventRecorder(opencode.client, query, sessionId, request.realtime)

    const body = {
      parts: [{ type: "text", text: request.prompt }],
    }
    if (request.agent) {
      body.agent = request.agent
    }
    if (request.format) {
      body.format = request.format
    }

    let response
    let messages
    try {
      response = await opencode.client.session.prompt({
        ...query,
        sessionID: sessionId,
        ...body,
      })
      messages = await opencode.client.session.messages({
        ...query,
        sessionID: sessionId,
      })
    } finally {
      await eventRecorder.stop()
    }

    const responseError = response?.error
    console.log(JSON.stringify({
      ok: !responseError,
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
      realtime: eventRecorder.info,
      response,
      messages,
      error: responseError
        ? {
            message: "opencode session.prompt returned error",
            response: responseError,
          }
        : null,
    }))
  } finally {
    opencode.server.close()
    await fetchDispatcher.close()
    // Safety net: if the event loop still has lingering handles after 3s, force exit.
    const forceExit = setTimeout(() => {
      console.error("Force exiting opencode runner after server close")
      process.exit(0)
    }, 3000)
    forceExit.unref()
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
