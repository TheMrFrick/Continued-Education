import React from 'react';
import './globals.css';

export const metadata = {
  title: 'Background Removal – Attempt 1',
  description: 'Drag & drop image background removal with @imgly/background-removal',
};

export default function RootLayout({ children }: { children: React.ReactNode }) {
  return (
    <html lang="en">
      <body className="min-h-screen bg-slate-50 text-slate-900">
        <div className="mx-auto max-w-5xl p-6">{children}</div>
      </body>
    </html>
  );
}
