# 🔌ESP32 Home Automation System  
### 4-Channel LED Control using ESP RainMaker & Amazon Alexa
---

## 1. Project Overview

This project demonstrates a cloud-enabled home automation prototype built using an ESP32 microcontroller.  
The system controls four independent loads, implemented using LEDs, and supports local, cloud, and voice-based control.

The ESP32 is powered directly via USB from a laptop, making the setup safe, compact, and suitable for laboratory and academic demonstrations.

Control methods supported:
- Physical push buttons
- Mobile application using ESP RainMaker
- Voice commands using Amazon Alexa

---

## 2. Key Features ✨

- Four independent LED-based loads
- ESP RainMaker cloud integration
- Amazon Alexa voice control
- Physical push-button toggle control
- EEPROM-based state persistence
- BLE-based Wi-Fi provisioning
- Wi-Fi connection status indication
- Real-time synchronization between hardware and cloud

---

## 3. System Architecture

### Block Diagram 
```
┌────────────────────┐
│ Amazon Alexa       │
│ (Voice Commands)   │
└─────────┬──────────┘
          │
┌─────────▼──────────┐
│ ESP RainMaker      │
│ Cloud Platform     │
└─────────┬──────────┘
          │
┌─────────▼──────────┐
│ ESP32 Controller   │
└─────────┬──────────┘
          │
┌─────────▼──────────┐
│ LEDs (4 Loads)     │
└────────────────────┘

Physical Push Buttons ──→ ESP32 GPIO  
EEPROM ────────────────→ State Storage  
USB (Laptop) ──────────→ Power Supply
```
---

## 4. Hardware Components

| Component | Quantity | Purpose |
|--------|----------|--------|
| ESP32 Development Board | 1 | Main controller |
| LEDs | 4 | Represent loads |
| Push Buttons | 4 | Manual control |
| Current-limiting Resistors | 4 | LED protection |
| Breadboard | 1 | Circuit assembly |
| Jumper Wires | As required | Connections |
| Laptop (USB) | 1 | Power source |

---

## 5. Power Supply🔋

- The ESP32 is powered only via USB from a laptop.
- No external adapters or high-voltage components are used.
- This ensures electrical safety and ease of demonstration.

---

## 6. GPIO Pin Configuration

### LED (Load) Pins

| Load | GPIO |
|----|----|
| LED 1 | GPIO 13 |
| LED 2 | GPIO 12 |
| LED 3 | GPIO 14 |
| LED 4 | GPIO 27 |

### Push Button Pins (INPUT_PULLUP)

| Button | GPIO |
|------|------|
| Button 1 | GPIO 18 |
| Button 2 | GPIO 19 |
| Button 3 | GPIO 21 |
| Button 4 | GPIO 22 |

### Status Indicator

| Function | GPIO |
|--------|------|
| Wi-Fi Status LED | GPIO 2 |

---

## 7. Software & Libraries Used

- ESP32 Arduino Core
- ESP RainMaker SDK
- WiFiProv (BLE provisioning)
- AceButton library
- EEPROM library

---

## 8. Functional Description

### 8.1 LED Load Control

Each LED acts as a logical appliance.  
The ESP32 directly controls LEDs through GPIO pins, making this project a safe prototype of a real home automation system.

---

### 8.2 Physical Push-Button Control

- Each push button toggles its corresponding LED.
- Implemented using the AceButton library.
- Features software debouncing and non-blocking operation.

Button press results in:
1. LED state toggle  
2. EEPROM update  
3. Cloud state synchronization  

---

### 8.3 EEPROM State Persistence💾

- LED states are stored in EEPROM.
- After power loss or reset, the ESP32 reads stored values and restores all LED states automatically.

| EEPROM Address | Load |
|--------------|------|
| 0 | LED 1 |
| 1 | LED 2 |
| 2 | LED 3 |
| 3 | LED 4 |

---

### 8.4 ESP RainMaker Cloud Control☁️

- Each LED is registered as a Switch device in ESP RainMaker.
- Allows remote ON/OFF control and real-time state monitoring.
- Cloud updates are reflected immediately on hardware.

---

### 8.5 Amazon Alexa Voice Control🗣️

- Devices are automatically discovered by Amazon Alexa.
- Example commands:
  - “Alexa, turn on Switch 1”
  - “Alexa, turn off Switch 3”

Voice commands propagate through:
Alexa → RainMaker Cloud → ESP32 → LED → EEPROM

---

### 8.6 Wi-Fi Provisioning (BLE)

- Initial Wi-Fi setup is performed via BLE.
- A QR code is displayed on the Serial Monitor.
- Credentials are securely transferred using the RainMaker mobile application.

---

## 9. Execution Flow

1. ESP32 powers on via USB  
2. EEPROM states are read  
3. LEDs are restored to previous states  
4. RainMaker node and devices initialize  
5. BLE provisioning starts (first-time setup)  
6. Normal operation begins  

---

## 10. Safety & Design Notes ⚠️

- No AC mains or relay modules are used
- LEDs replace real appliances for safety
- Suitable for laboratory experiments and academic evaluation

---

## 11. Future Scope

- Replace LEDs with relay modules for real appliances
- Add OTA firmware updates
- Integrate energy monitoring

---

## 12. Conclusion

This project demonstrates a modern IoT-based home automation system using an ESP32, integrating physical control, cloud connectivity, voice interaction, and non-volatile memory in a safe and structured implementation.

---
