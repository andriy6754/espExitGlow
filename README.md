# espExitGlow

An ESP32-based smart status board built with **ESP-IDF** and **FreeRTOS** that gives you an at-a-glance summary before heading out the door:

- **Weather Animations** — Displays pixel-art weather scenes (sunny, cloudy, drizzle, rain, shower, thunder, snow) on an 8×8 WS2812 RGB LED matrix/strip using [WeatherAPI](https://www.weatherapi.com/).
- **Live Digital Clock** — Shows the current time (`HH:MM`) on a TM1637 4-digit 7-segment display, synced via SNTP (`pool.ntp.org`).
- **Air Raid Alert Indicator** — Polls real-time Ukrainian air raid alerts from [tryvoha.online](https://tryvoha.online) and lights up a WS2812 RGB LED strip by alert level.

---

## Features

| Module | Output Hardware | Data Source | Update Interval |
| --- | --- | --- | --- |
| **Air Alert** | 12-LED WS2812 strip | `https://tryvoha.online/api/v1/alerts/<region>` | Every 30 s (5 s retry on error) |
| **Weather** | 64-LED (8×8) WS2812 matrix/strip | `https://api.weatherapi.com/v1/current.json` | Every 30 min (cached) |
| **Clock** | TM1637 4-digit 7-segment display | SNTP (`pool.ntp.org`, UTC+2) | Every 1 s |

### Air Alert Color Coding

| Alert Status | LED Strip Color | Meaning |
| --- | --- | --- |
| `AIR_ALERT_NONE` | Green (`0, 255, 0`) | No active air alert |
| `AIR_ALERT_YELLOW` | Yellow (`255, 255, 0`) | Yellow alert (e.g. drone threat) |
| `AIR_ALERT_RED` | Red (`255, 0, 0`) | Active red air alert |

---

## Hardware & Wiring

Designed for an **ESP32** board (e.g., ESP32-DevKitC V4):

| Component | Pin | ESP32 GPIO |
| --- | --- | --- |
| **WS2812 Strip #1** (Air Alert, 12 LEDs) | `DIN` | `GPIO 4` |
| **WS2812 Strip #2** (Weather 8×8, 64 LEDs) | `DIN` | `GPIO 5` |
| **TM1637 Display** (Clock) | `CLK` | `GPIO 16` |
| **TM1637 Display** (Clock) | `DIO` | `GPIO 17` |

Power all peripherals with `5V` and common `GND` (for physical builds with many LEDs at full brightness, use an external 5V power supply with a shared ground).

---

## Prerequisites

- **ESP-IDF** `v5.2+` (developed and locked with `v6.1.0`)
- **CMake** `3.22+`
- A free API key from [WeatherAPI](https://www.weatherapi.com/)
- *(Optional)* [Wokwi for VS Code](https://docs.wokwi.com/vscode/getting-started) for running the included hardware simulation

### Managed Components

Automatically fetched by the ESP-IDF Component Manager (`src/idf_component.yml`):

- `espressif/led_strip` — RMT-driven WS2812 LED strip driver
- `espressif/cjson` — JSON parsing for WeatherAPI and Tryvoha responses
- `heronet/tm1637` — TM1637 4-digit 7-segment display driver

---

## Configuration

### 1. Environment Variables (`.env`)

Copy `.env.example` to `.env` in the project root and add your WeatherAPI key:

```bash
cp .env.example .env
```

```env
WEATHER_API_KEY=your_weatherapi_key_here
```

`src/CMakeLists.txt` automatically reads `.env` and injects its variables as compile definitions.

### 2. Wi-Fi Credentials

Update `WIFI_SSID` and `WIFI_PASS` in `src/wifi.c`:

```c
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""
```

> The default `Wokwi-GUEST` credentials work out of the box in Wokwi simulation.

### 3. Location & Timezone

- **Weather city**: Change `CITY` in `include/weather.h` (default: `"Kyiv"`).
- **Air alert region**: Change `REGION_SLUG` in `src/air_alert.c` (default: `"kyiv"`; accepts any Tryvoha.online region slug such as `"kyiv"` or `"kyivska"`).
- **Timezone**: `src/clock.c` currently applies a fixed `UTC+2` offset.

---

## Building & Flashing

1. Make sure your ESP-IDF environment is exported (`. $IDF_PATH/export.sh`).
2. Set the target and build:

   ```bash
   idf.py set-target esp32
   idf.py build
   ```

3. Flash to your ESP32 and open the serial monitor:

   ```bash
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

   *Replace `/dev/ttyUSB0` with your board's serial port*

---

## Running in Wokwi Simulation

The repository includes `wokwi.toml` and `diagram.json` along with `sdkconfig.defaults` tuned for fast TLS handshakes in simulation (240 MHz CPU, mbedTLS performance flags, and cross-signed certificate bundle support for Cloudflare/GTS Root R4):

1. Build the firmware with `idf.py build`.
2. Open the project in VS Code with the **Wokwi Simulator** extension installed.
3. Run **Wokwi: Start Simulator** from the Command Palette.

---

## Project Structure

```text
espExitGlow/
├── CMakeLists.txt          # Top-level ESP-IDF CMake configuration
├── sdkconfig.defaults      # ESP32 target & mbedTLS / Wokwi defaults
├── .env.example            # Template for WEATHER_API_KEY
├── diagram.json            # Wokwi circuit diagram
├── wokwi.toml              # Wokwi firmware/ELF configuration
├── include/
│   ├── air_alert.h         # Air alert levels & task interface
│   ├── clock.h             # UTC+2 hour/minute getters
│   ├── display.h           # TM1637 clock display task
│   ├── led.h               # WS2812 pinout & LED strip helpers
│   ├── led_screen.h        # 8×8 weather screen types, palettes & frames
│   ├── weather.h           # WeatherAPI getters & city config
│   └── wifi.h              # Wi-Fi station init
└── src/
    ├── CMakeLists.txt      # Component registration & .env loader
    ├── idf_component.yml   # Managed component dependencies
    ├── main.c              # Entry point: LED init, Wi-Fi, SNTP, FreeRTOS tasks
    ├── air_alert.c         # Tryvoha.online HTTPS client & alert LED task
    ├── clock.c             # Time conversion from system epoch
    ├── display.c           # TM1637 HH:MM rendering task
    ├── led.c               # RMT WS2812 initialization & pixel control
    ├── led_screen.c        # 8×8 weather animation task
    ├── colors.c            # RGB color palettes for weather scenes
    ├── first_frames.c      # Initial 8×8 pixel-art frames (rain, snow, sun, etc.)
    ├── weather.c           # WeatherAPI HTTPS client with 30-min caching
    └── wifi.c              # ESP-IDF Wi-Fi STA connection handler
```
