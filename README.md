# T-LAB JARVIS

A compact 3D-printed desktop AI assistant powered by the Seeed XIAO ESP32-S3 Sense.

## Features

- Jarvis wake word
- AI voice conversations
- Animated emotional eyes
- Automatic standby
- Wi-Fi connectivity
- Customizable AI personality

## Hardware

- Seeed XIAO ESP32-S3 Sense
- SH1107 128x128 OLED display
- MAX98357A I2S amplifier
- Speaker
- Mini breadboard
- Jumper wires
- 2.4 GHz antenna
- Custom 3D-printed enclosure

## Wiring

| Component | Pin | XIAO |
|---|---|---|
| OLED | SDA | D0 / GPIO1 |
| OLED | SCL | D1 / GPIO2 |
| OLED | VCC | 3V3 |
| OLED | GND | GND |
| MAX98357A | DIN | D2 / GPIO3 |
| MAX98357A | BCLK | D4 / GPIO5 |
| MAX98357A | LRC | D10 / GPIO9 |
| MAX98357A | VIN | 5V |
| MAX98357A | SD | 3V3 |
| MAX98357A | GND | GND |

Leave GAIN unconnected.
Connect the speaker across SPK+ and SPK-.
The onboard microphone requires no external wiring.

## Firmware

Download the merged BIN from GitHub Releases.

- Target: ESP32-S3
- Flash size: 8 MB
- Flash mode: DIO
- Flash frequency: 80 MHz
- Flash address: 0x0

The merged release binary was successfully built but has not been independently flash-tested.

## Build from source

Requires ESP-IDF v5.5.6.

    git submodule update --init --recursive
    cp sdkconfig.release sdkconfig
    idf.py build

## AI Service

AI conversations require Wi-Fi and a compatible Xiaozhi backend.
Personality settings are configured through the AI service.

## Credits

Based on Xiaozhi ESP32 v2.2.2:
https://github.com/78/xiaozhi-esp32

Custom firmware, animations and enclosure by T-LAB 3D.

Original enclosure mounting references: Kira project.

Upstream license notices remain applicable.

