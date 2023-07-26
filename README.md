# Project: Wireless Weather Station

# WeatherStation-OutdoorUnit-BlackPill

This project presents a weather monitoring system, composed of an indoor unit and an outdoor unit. Each unit is equipped with unique features, yet they work together to deliver an immersive weather tracking experience. Each module, hosted in separate repositories, is equipped with distinct features.

## [WeatherStation-IndoorUnit-BlackPill](https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill.git)
The indoor unit is the main control hub, built around the Blackpill F411 microcontroller, offering the following key features:

- **Display**: This unit is equipped with a 3.2" TFT touch display, supported by the LVGL library.
- **Connectivity**: The unit communicates with the external (outdoor) sensor via ESP8266 using AT commands. It supports two operational modes: hotspot (acting as an AP for the sensor) and router (communicating via an external router). The unit automatically switches to hotspot mode if router connection is lost.
- **Communication Redirection**: It redirects incoming UART communication from the ESP8266 to an external UART device, such as a computer.
- **Sensing Capabilities**: Using the SCD41 sensor in low-energy mode, it measures CO2, humidity, and temperature.
- **Configuration**: It allows users to change WiFi settings, which are also synchronized with the external unit.
- **Power**: Battery-powered with a built-in battery level monitoring feature.
- **Debugging**: It includes a terminal that displays UART communication with the ESP module.
- **Data Logging**: Measurement data is logged on an SD card.
- **Data Visualization**: The unit can generate graphs of the measured values for the last three days.
- **Time Keeping**: Users can set the date manually.
- **Power Management**: It boasts low energy consumption by switching to standby mode when no measurements are being taken or the display hasn't been touched for a sufficient period. It wakes up for measurements without engaging the display and can be awakened/activate the display with a touch.

## [WeatherStation-OutdoorUnit-BlackPill](https://github.com/fradocki/WeatherStation-OutdoorUnit-BlackPill.git)
The outdoor unit, built around the Blackpill F401, acts as the weather sensor with these prominent features:

- **Connectivity**: It communicates with the indoor unit via ESP8266 using AT commands, operating in hotspot or router mode, and auto-switching to hotspot mode when router connection is lost.
- **Communication Redirection**: It redirects incoming UART communication from the ESP8266 to an external UART device, such as a computer.
- **Display**: This unit also comes with a touch display, supported by the LVGL library.
- **Sensing Capabilities**: It uses the BME280 sensor in low-energy mode for measuring humidity, temperature, and pressure, and the GP2Y1010AU0F sensor to measure particulate matter.
- **Configuration**: The WiFi settings can be adjusted by changing the network on the indoor unit.
- **Power Management**: It has a low power consumption profile by transitioning to standby mode and turning on only during measurement. Battery level monitoring is also provided.

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/c2fbab01-6a3c-47d0-970f-0ed23f343249" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/f92b3d7e-176b-4ab2-891d-91edc39cbb0b" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/bf9612a5-ef08-4857-9456-698a314063c8" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/87eb6798-ac57-404c-976b-7ecbd8857f24" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/0c327a05-8f82-4697-902b-c4e50f2af104" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/ce926832-36b2-4533-b0e5-2a37f0be6413 " width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/241dd0da-666a-42b9-b73c-86475aa9020d" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/96035a98-1bee-491b-af14-2b5ac1c86671 " width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/6dd89055-be23-41be-83e3-b327c169c332" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/a3b44ca1-caff-4249-b22f-35c7208063d7 " width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/9e8d90b1-55c8-43e6-b4cb-b692de571956" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/1c2d82c0-4730-42b3-abea-df85912364a4" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/33b99508-f09c-4152-9003-b27c9082bd0a" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/8ac5778c-763f-4ec0-8b61-8478141404f8" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/30180c79-51f7-4ef6-9a3a-bf1bfd839106" width="800">

<img src="https://github.com/fradocki/WeatherStation-IndoorUnit-BlackPill/assets/106407252/ce2ecd0c-8a9f-486c-96fb-d161a7d27c9e" width="800">
