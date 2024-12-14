# Gree Air Conditioner Control with BMP180 Barometric Pressure/Temperature/Altitude Sensor

## Overview
This project uses an ESP8266 microcontroller to control a Gree air conditioner (AC) based on temperature readings from a BMP180 sensor. The system automatically manages the AC's power state (on/off) depending on the ambient temperature.

### Features
- **Automatic Temperature-Based Control for Energy Efficiency**
  - Turns the AC on when the temperature exceeds 31.45°C.
  - Turns the AC off when the temperature drops below 29.2°C.
- **Energy Efficiency**
  - Enters deep sleep mode for 15 seconds when the temperature is stable or near thresholds to conserve energy.
- **Seamless IR Communication**
  - Sends commands to a Gree AC using an IR LED.

### Components
- **ESP8266/ESP32 Microcontroller**
- **BMP180 Temperature Sensor**
- **IR LED** (for sending IR signals to the AC)

---

## Code Explanation

### Dependencies
- **IRremoteESP8266**: Handles IR communication.
- **IRsend**: Sends IR signals.
- **ir_Gree**: Provides Gree AC-specific control functionality.
- **SFE_BMP180**: Interfaces with the BMP180 sensor for temperature readings.
- **Wire**: Facilitates I2C communication with the BMP180.

### Key Variables
- **kIrLed**: GPIO pin for the IR LED.
- **T**: Stores the temperature reading from the BMP180.
- **status**: Indicates the success or failure of BMP180 operations.

---

### Setup
1. **Initialize the IR and BMP180 modules.**
2. **Configure the AC settings as needed.**

---

### Main Loop
1. Start a temperature measurement with the BMP180.
2. Check if the temperature:
   - **Exceeds 31.45°C**: Turns the AC on and sends the IR signal.
   - **Drops below 29.2°C**: Turns the AC off and sends the IR signal.
   - **Stabilizes near thresholds (±0.3°C)**: Enters deep sleep for stability.
3. Default behavior: Enters deep sleep for 150 seconds to conserve power.

---

## Setup Instructions

### Include Libraries
Make sure the following libraries are included in your Arduino IDE:
- `IRremoteESP8266`
- `IRsend`
- `ir_Gree`
- `Wire`
- `SFE_BMP180`

### Connect the Components
- **BMP180 Sensor**: Connect to the ESP8266/ESP32 using I2C.
- **IR LED**: Wire to the specified GPIO pin for sending IR signals.

### Upload the Code
1. Open the provided code in the Arduino IDE.
2. Upload it to your ESP8266/ESP32 microcontroller.

---

## Usage
1. Place the ESP device near the Gree AC unit, ensuring the IR LED faces the receiver.
2. Power the ESP8266.
3. Observe automatic AC control based on the ambient temperature.

---
