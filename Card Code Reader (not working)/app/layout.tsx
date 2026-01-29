import type { Metadata } from 'next'
import './globals.css'

export const metadata: Metadata = {
  title: 'Card Code Reader',
  description: 'Camera OCR for alphanumeric card codes',
}

export default function RootLayout({ children }: { children: React.ReactNode }) {
  return (
    <html lang="en">
      <body>
        <div className="min-h-screen p-4 md:p-6 lg:p-8">
          <div className="mx-auto max-w-6xl">
            {children}
          </div>
        </div>
      </body>
    </html>
  )
}
