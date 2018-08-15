# Introduction

Arduino project to configure the ESP8266 microcontroller to read the swimming pool temperature every N minutes using DS18B20 sensor.

The temperature data is sent through a WIFI connection to ThingSpeak service to can visualise it.

## Prerequisites

Install ESP8266 libraries in Arduino IDE.

Create a config.h file with these fields:

```
#define _FW_HOST "YOUR HOST WHERE IS FOTA files"
#define _FW_URL_VERSION "YOUR HTTP URL TO DOWNLOAD THE VERSION FOTA FILE"
#define _FW_URL_IMAGE "YOUR HTTP URL TO DOWNLOAD THE BINARY FOTA FILE"
#define _THINGSPEAK_APIKEY "YOUR THINSPEAK APIKEY"
#define _MY_SSID "YOUR WIFI SSID"
#define _MY_PWD "YOUR WIFI PASSWORD"
```

# Use

Use Arduino IDE (or similar) to compile code and deploy to ESP8266 microcontroller (NodeMCU, ESP-12E, ESP-12F etc.)

# Notes

If use NodeMCU board the deploy is easy but it's only a good idea for prototyping. 

If you are thinking to make a real sensor you need remove some parts or NodeMCU board that speed a lot of energy (UART, Regulator, etc.) or better directly use an ESP12-F (o similar). In this way, you can power it with a battery for 1-2 years:

- [ESP8266 Tips](https://www.instructables.com/id/ESP8266-Pro-Tips/)
- [MCP1700](http://ww1.microchip.com/downloads/en/DeviceDoc/20001826D.pdf) Overall current consumption in DeepSleep is ~28µA (ESP:~26µA, MCP:~2µA)
- [ESP8266 Voltage Regulator (LiPo and Li-ion Batteries)](https://randomnerdtutorials.com/esp8266-voltage-regulator-lipo-and-li-ion-batteries/)
- [ESP8266 Deep Sleep](https://randomnerdtutorials.com/esp8266-deep-sleep-with-arduino-ide/)
- [Programming ESP8266 using Arduino UNO](https://myelectronicslab.com/programming-esp8266-using-arduino-uno/)
- [Program ESP8266 ESP-12E With Arduino](https://www.hackster.io/harshmangukiya/program-esp8266-esp-12e-with-arduino-using-ftdi-cable-2310c9)
- [Programming ESP8266 ESP-12E](https://circuiteasy.com/programming-wi-fi-module-esp-8266/)
