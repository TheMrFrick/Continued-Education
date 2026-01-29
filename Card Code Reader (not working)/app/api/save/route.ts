import { NextResponse } from 'next/server'
import { promises as fs } from 'fs'
import path from 'path'

export async function POST(request: Request) {
  try {
    const { content } = await request.json() as { content?: string }
    const text = (content ?? '').toString()
    const root = process.cwd()
    const dir = path.join(root, 'data')
    const file = path.join(dir, 'codes.txt')
    await fs.mkdir(dir, { recursive: true })
    const append = text.endsWith('\n') ? text : text + '\n'
    await fs.appendFile(file, append, 'utf8')
    return NextResponse.json({ ok: true })
  } catch (err: any) {
    console.error('Save failed', err)
    return NextResponse.json({ ok: false, error: err?.message ?? 'Unknown error' }, { status: 500 })
  }
}
