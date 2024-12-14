# BorsaMatik/StocksMatik

**BorsaMatik/StocksMatik** is an ESP32-based project that integrates an RFID card system with WiFi connectivity and an LCD display. The device fetches and displays live data and percentages from APIs for selected stock markets or other monitored entities. This project is designed for real-time data visualization with efficient power management.

*Note: DARDANEL, DNISI, and ECZACI used in the code are Turkish company names.*

---

## Features
- **RFID Authentication**: Recognizes authorized RFID cards to initiate operations.
- **WiFi Connectivity**: Connects to WiFi to fetch live data from specified APIs.
- **LCD Display**: Displays live data and corresponding percentages for selected entities.
- **User Interaction**: Two buttons allow navigation between different entities' data.
- **Power Management**:
  - Automatically enters deep sleep mode to conserve power after inactivity.
  - Sleep mode can be disabled with a dedicated switch button.

---

## Components Used
- **ESP32 Microcontroller**
- **MFRC522 RFID Reader Module**
- **LiquidCrystal_I2C LCD Display** (16x2)
- **Buttons**: For navigation (next and previous entity)
- **WiFi Connectivity**

---

## How It Works

### Startup & Authentication
1. Upon startup, the device waits for an authorized RFID card.
2. Displays a personalized welcome message if the card is recognized.

### Data Fetching
1. Connects to the specified WiFi network.
2. Fetches live data from predefined API endpoints for selected entities.

### User Interaction
1. Users can navigate between different entities using the "Next" and "Previous" buttons.
2. The data and corresponding percentage are displayed on the LCD.

### Power Management
1. If no interaction is detected within a set time, the device enters deep sleep mode.
2. Wakes up on button press.

---

## Code Overview

### Configuration
- **WiFi Credentials**: Update `ssid` and `password` with your network details.
- **API Endpoints**: Replace `apiURLdrd`, `apiURLdn_isi`, etc., with the desired API URLs.

### Main Functions
- `viewStock(int stock)`: Fetches and displays data for the selected entity.
- `backInterrupt()` and `forwardInterrupt()`: Handle button press events for navigation.
- Power management is achieved using `esp_deep_sleep_start()` after inactivity.

---

## Pin Assignments

| **Component**    | **Pin** |
|-------------------|---------|
| RFID SS Pin       | 5       |
| RFID RST Pin      | 15      |
| Button (Next)     | 13      |
| Button (Previous) | 4       |
| Sleep Switch      | 16      |

---

## How to Run

### Setup Hardware
1. Connect components as per the pin assignments.
2. Ensure your ESP32 is properly flashed with the necessary libraries:
   - `WiFi.h`
   - `HTTPClient.h`
   - `LiquidCrystal_I2C.h`
   - `MFRC522.h`

### Upload Code
1. Update the WiFi credentials and API endpoints in the code.
2. Upload the code to the ESP32 using the Arduino IDE.

### Interact
1. Power the device.
2. Use the RFID card to authenticate.
3. Navigate data using the buttons.

---

## Example Output

### On Startup:
BORSAMATIK KART BEKLENIYOR (WAITING FOR A BORSAMATIK CARD)

### After Authentication:
WiFiYA BAGLANDI! (CONNECTED TO WIFI!)
BIR BUTONA BASIN (PRESS A BUTTON)

### Stocks Data:
1-DARDANEL CANLI (DARDANEL LIVE)
23.45 %15.67

