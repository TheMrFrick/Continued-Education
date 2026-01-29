import type { Config } from 'tailwindcss'

const config: Config = {
  content: [
    "./app/**/*.{ts,tsx,css}",
    "./components/**/*.{ts,tsx}",
    "./app/globals.css"
  ],
  theme: {
    extend: {
      colors: {
        roi: '#ffffff'
      }
    }
  },
  plugins: []
}
export default config
