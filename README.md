# 🌿 ESP8266 Telegram Bot Monitoring (DHT22 + Soil Moisture Sensor)

This project implements a smart plant monitoring system using an **ESP8266 (NodeMCU)**, a **DHT22** sensor for temperature and humidity, and a **soil moisture sensor**. The sensor data is sent to a **Telegram Bot**, and can be controlled remotely using the `/on` and `/off` commands.

---

## 🔧 Hardware Requirements

- ESP8266 (NodeMCU)
- DHT22 Sensor
- Soil Moisture Sensor:
  - Digital output (connected to D3 / GPIO0)
  - Analog output (connected to A0)
- Jumper wires
- Breadboard
- WiFi connection

---

## 📌 Pin Configuration

| Component                  | ESP8266 Pin |
|----------------------------|-------------|
| DHT22 (data pin)           | D4 (GPIO2)  |
| Soil Moisture Sensor (D)   | D3 (GPIO0)  |
| Soil Moisture Sensor (A)   | A0          |

---

## 🛠️ Required Libraries (Arduino IDE)

Install the following libraries via Library Manager:

- `DHT sensor library` by Adafruit
- `Adafruit Unified Sensor`
- `UniversalTelegramBot`
- `WiFiClientSecure`

---

## ⚙️ Setup

Update these lines in the code with your WiFi and Bot credentials:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
