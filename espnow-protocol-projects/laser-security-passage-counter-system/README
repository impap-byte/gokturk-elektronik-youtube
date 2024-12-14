# ESP-NOW Communication System for Burglar Detection and Laser Pass Counter  

## Overview  
This project demonstrates the use of ESP-NOW communication between two ESP8266/ESP32 devices. It integrates an infrared (IR) remote, light sensor, and an I2C LCD to create a system capable of:  

- **Burglar Detection Mode**: Detects motion and alerts with a buzzer.  
- **Laser Pass Counter**: Counts the number of times a laser beam is interrupted.  

The system has two main components:  
1. **Transmitter**: Monitors inputs and sends data to the receiver.  
2. **Receiver**: Displays the received data on an I2C LCD.  

---

## Transmitter  

### Features  
- **IR Remote Integration**: Enables or disables burglar mode using an IR remote.  
- **Laser Detection**: Uses a light sensor (photoresistor) to detect changes in light intensity.  
- **Buzzer Alerts**: Activates a buzzer when motion or light interruption is detected in burglar mode.  
- **ESP-NOW Communication**: Sends data to the receiver, including:  
  - Laser pass count.  
  - Burglar mode status.  

### Components  
- **IR Receiver**: Connected to D1.  
- **Photoresistor**: Connected to A0.  
- **Buzzer**: Connected to D2.  

### Key Variables  
- `myData.laser`: Tracks the number of laser beam interruptions.  
- `myData.burglar_mode`: Boolean indicating if burglar mode is active.  

### Code Flow  
1. Initializes WiFi in station mode and sets up ESP-NOW communication.  
2. Monitors the IR remote to toggle burglar mode.  
3. Continuously reads the photoresistor to detect laser beam interruptions.  
4. Sends updated data to the receiver via ESP-NOW.  

---

## Receiver  

### Features  
- **ESP-NOW Communication**: Receives data from the transmitter.  
- **LCD Display**: Shows the current state:  
  - **"BURGLAR MODE ACTIVATED"** when burglar mode is enabled.  
  - **"ENTRY DETECTED"** if motion is detected in burglar mode.  
  - **"Number of Passes"** and the laser pass count when burglar mode is disabled.  

### Components  
- **I2C LCD**: Address `0x27`, 16x2 character display.  

### Key Variables  
- `myData.laser`: Received laser pass count.  
- `myData.burglar_mode`: Received burglar mode status.  

### Code Flow  
1. Initializes WiFi in station mode and sets up ESP-NOW communication.  
2. Processes incoming data from the transmitter via a callback.  
3. Updates the LCD display based on the received data.  

---

## Usage Instructions  

### Setup the Transmitter  
1. Upload the transmitter code to an ESP8266/ESP32.  
2. Connect the following components:  
   - IR Receiver to **D1**.  
   - Photoresistor to **A0**.  
   - Buzzer to **D2**.  

### Setup the Receiver  
1. Upload the receiver code to another ESP8266/ESP32.  
2. Connect the I2C LCD display (address `0x27`) to the receiver.  

### Test the System  
1. Use the IR remote to toggle burglar mode.  
2. Interrupt the laser beam to simulate entries or counts.  
3. Observe the following behaviors:  
   - Buzzer alerts when motion or light interruption is detected.  
   - LCD updates showing the burglar mode status or laser pass count.  

---

## Example Outputs  

### Burglar Mode Activated  
On the LCD:  
BURGLAR MODE ACTIVATED

If motion is detected in this mode:  
ENTRY DETECTED

### Laser Pass Counter  
When burglar mode is disabled, the LCD displays:  
Number of Passes: X

Where `X` is the laser pass count.
---
