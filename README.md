TL;DR : There is a ".bin" file in the BUILD directory that you can directly flash to the TY51822r3

# BLE_Server_BME280
mbed application for BLE server on TY51822r3 with a builtin BLE shield and Bosch BME280 sensor

This application needs to be flashed into a TY51822r3 running mbed firmware.

The instractions for updating firmware is at here: https://developer.mbed.org/teams/Switch-Science/wiki/Firmware-Switch-Science-mbed-TY51822r3 

The latest firmware can be found here: https://developer.mbed.org/media/uploads/asagin/lpc11u35_sscity_if_crc.bin

The  BME280 sensor is an environmental sensor from Bosch, which measures temperature, humidity and pressure.

You'll also need a couple of jumpers, USB cables etc.

Please find the BLE Gatt Client application under https://github.com/soramame21/BLEClient_mbedDevConn repo.

You can verify the output of this application either:
  - on a serial terminal for your respective OS 
    OR
  - on a BLE enabled Android device with the Nordic nRF connect app

The application will print a few messages on the serial terminal and start reading the light sensor and show the readings with a 1sec delay.

If you are using the Nordic android app, you can scan for the "BME280" device and "connect" to it. Upon successful discovery of services, you can enable "notifications" and start getting the light sensor's data in hex.
