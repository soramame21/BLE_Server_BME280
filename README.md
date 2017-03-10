# BLE_Server_BME280
This mbed application for BLE GATT server running over TY51822r3 with a builtin BLE shield and wired Bosch BME280 sensor. it broadcasts measured value of temperature, humidity and pressure to BLE GATT client.

# Getting started with it on mbed OS

This is a very simple guide, reviewing the steps required to get BLE_Server_BME280 working on an mbed OS platform.
Please install [mbed CLI](https://github.com/ARMmbed/mbed-cli#installing-mbed-cli).

## Get the example application!

From the command line, import the example:

```
git clone https://github.com/soramame21/BLE_Server_BME280
cd BLE_Server_BME280
mbed deploy
```

### Now compile

Invoke `mbed compile` specifying the name of your platform and your favorite toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

```
mbed compile -m TY51822r3 -t ARM
```

Your PC may take a few minutes to compile your code. At the end you should get the following result:

```
[snip]
Link: BLE_Server_BME280
Elf2Bin: BLE_Server_BME280
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Misc                  | 17384 |    29 |  404 |
| drivers               |  1489 |     8 |   56 |
| features/FEATURE_BLE  | 12796 |    38 |  512 |
| hal                   |   558 |    16 |    0 |
| platform              |  1759 |     8 |   92 |
| rtos                  |   598 |     8 |    0 |
| rtos/rtx              |  6598 |   100 | 5036 |
| targets/TARGET_NORDIC | 23070 |   270 | 2423 |
| Subtotals             | 64252 |   477 | 8523 |
+-----------------------+-------+-------+------+
Allocated Heap: unknown
Allocated Stack: unknown
Total Static RAM memory (data + bss): 9000 bytes
Total RAM memory (data + bss + heap + stack): 9000 bytes
Total Flash memory (text + data + misc): 64729 bytes

Object file test_env.o is not unique! It could be made from: C:\iotBu\myrepo\BLE_Server_BME280\mbed-os\features/unsupported\tests\mbed\env\test_env.cpp .\mbed-os\features\frameworks\greentea-client\source\test_env.cpp
Image: .\BUILD\TY51822r3\ARM\BLE_Server_BME280.hex

```

### Program your board

1. Connect your mbed device to the computer over USB.
1. Copy the binary file to the mbed device .
1. Press the reset button to start the program.

You should see the LED of your platform turning on and off.

Congratulations if you managed to complete this test!

## Required hardware
* [TY51822r3 target platform](https://developer.mbed.org/platforms/Switch-Science-mbed-TY51822r3/)

    It is a Bluetooth low energy development board with the Nordic's nRF51822 Rev.3 SoC.

    [Instractions for updating firmware](https://developer.mbed.org/teams/Switch-Science/wiki/Firmware-Switch-Science-mbed-TY51822r3)

    [Latest firmware version](https://developer.mbed.org/media/uploads/asagin/lpc11u35_sscity_if_crc.bin)

* [BME280 sensor](https://developer.mbed.org/components/BME280-Combined-humidity-and-pressure-se/)

* Breadboard, wires, 2 registers (2.2k-ohm) and a USB cable.

## Wiring
BME280 uses I2C interface (SDI/SCK) to communicate with TY51822r3. please refer following table about paired pin wiring between BME280 and TY51822r3.
```
  +------------+----------------+
  | BME280 Pin | TY51822r3 Pin  |
  +------------+----------------+
  |  1 SDO     |      GND       |
  |  2 SCK     |  P0_7 scl I2C0 |
  |  3 SDI     | P0_30 sda I2C0 |
  |  4 CSB     |      VDD       |
  |  5 GND     |      GND       |
  |  6 Vcore   |      VDD       |
  |  7 Vio     |      VDD       |
  +------------+----------------+

```
* SDI and SCK must be pulled-up by 2.2k-ohm registers.


## Overview
![Overview of Demo](myImageBME280.png)



Please find the BLE GATT Client application under https://github.com/soramame21/BLEClient_mbedDevConn repo.

You can verify the output of this application either:
  - on a serial terminal for your respective OS

    BME280 data are read and print every second.

    OR
  - on a BLE enabled Android device with the Nordic nRF connect app

    You can scan for the "BME280" device and "connect" to it. Upon successful discovery of services, you can enable "notifications" and start getting the sensor's data.
