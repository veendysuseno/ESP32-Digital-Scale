# ESP32 Digital Scale

This project is an IoT-based digital scale system using the ESP32 microcontroller. It integrates various components such as a load cell (HX711), LCD I2C display, WiFi, HTTP, MQTT, and push buttons for user interaction. The system measures weight using the HX711 load cell, displays it on an LCD, and sends the data to a remote server via HTTP and MQTT.

## Features

- **Real-time weight measurement** using the HX711 load cell sensor.
- **Display weight on LCD** in kilograms.
- **WiFi connectivity** for sending data to a web server via HTTP.
- **MQTT communication** for publishing weight data to a broker.
- **Tare functionality** using a physical button to reset the scale to zero.
- **Data transmission** to a remote server with HTTP POST requests when triggered by a button press.

## Components Used

- **ESP32**: The main microcontroller.
- **HX711 Load Cell**: For weight measurement.
- **LCD I2C Display**: 16x2 display to show weight.
- **Push Buttons**: For tare and sending data.
- **WiFi & MQTT**: For IoT functionality.

## Libraries

The following libraries are used in this project:

- [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
- [HTTPClient](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient)
- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [HX711](https://github.com/bogde/HX711)
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

## Circuit Diagram

1. **HX711 Load Cell**:
   - `DOUT` connected to GPIO 16
   - `SCK` connected to GPIO 4
2. **Push Buttons**:
   - Button 1 (Tare) connected to GPIO 27
   - Button 2 (Send Data) connected to GPIO 26
3. **LCD I2C**:
   - `SDA` connected to GPIO 21
   - `SCL` connected to GPIO 22

## How It Works

1. **Startup**: The ESP32 connects to the specified WiFi network and prepares for MQTT communication.
2. **Measurement**: The HX711 load cell measures the weight, which is displayed on the LCD in real-time.
3. **Tare Button**: Pressing the tare button (connected to GPIO 27) resets the weight reading to zero.
4. **Data Transmission**: Pressing the send button (connected to GPIO 26) sends the weight to the server using an HTTP POST request and publishes it via MQTT.
5. **LCD Display**: The weight is shown on the LCD display, updating only when the weight changes.

## Code Overview

- **WiFi & HTTP Setup**: The ESP32 connects to WiFi using the provided credentials and sends data via HTTP POST requests.
- **MQTT Communication**: The ESP32 publishes weight data to an MQTT topic when the weight changes.
- **HX711 Load Cell**: The scale reads the load cell data, processes it to kilograms, and handles tare functionality.
- **Push Buttons**: One button resets the scale (tare), and the other sends the weight data to the server and MQTT.

### Important Variables:

- `ssid` and `password`: WiFi credentials.
- `serverName`: The endpoint for HTTP POST requests.
- `mqtt_server`, `mqtt_port`: MQTT broker settings.
- `topic1`: MQTT topic for publishing weight data.

## Installation

### Requirements

1. ESP32 board setup in Arduino IDE.
2. Libraries:
   - `WiFi`, `HTTPClient`, `PubSubClient`, `HX711`, `LiquidCrystal_I2C`.

### Steps

1. Clone this repository.
2. Open `ESP32_Digital_Scale.ino` in Arduino IDE.
3. Install required libraries.
4. Update the WiFi credentials and server URLs in the code.
5. Upload the code to your ESP32.

## Usage

- Power on the ESP32 and wait for it to connect to WiFi.
- The current weight is displayed on the LCD screen.
- Press Button 1 to tare (reset the scale to zero).
- Press Button 2 to send the weight data to the server (HTTP) and MQTT broker.

## Troubleshooting

- **WiFi not connecting**: Ensure the credentials are correct and the server is reachable.
- **No weight displayed**: Check the HX711 connections and calibration.
- **HTTP or MQTT not working**: Make sure the server and broker settings are correctly configured and accessible.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

For any questions or suggestions, feel free to reach out at [veendysuseno@gmail.com](veendysuseno@gmail.com).
