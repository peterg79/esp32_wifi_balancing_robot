# ESP32_Wifi_Balancing_Robot
 An Self Balancing Robot based ESP32 can be controlled use Android over Wifi

This self-balancing robot is based on [B-Robot][https://www.jjrobots.com/much-more-than-a-self-balancing-robot/], which is open sourced on jjrobots. Thanks to the article by "[ghmartin77][https://www.jjrobots.com/community/migrated-forums-4-jjrobots-b-robot/esp32-port-of-b-robot_evo2-code-3/]" posted on the B-Robot forum.

This repository is a fork of [bluino/esp32_wifi_balancing_robot](https://github.com/bluino/esp32_wifi_balancing_robot)

To compile, install [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer), [AsyncTCP](https://github.com/ESP32Async/AsyncTCP) and [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) libraries.
Then, copy [secret-template.h](secret-template.h) to `secret.h` and edit credentials there.
Install [littlefs-uploader](https://github.com/earlephilhower/arduino-littlefs-upload) and upload static content (`index.html` and `index.css`) to the ESP32.

Then you're ready to compile and upload the firmware.

##Pairing a PS3 controller
Please follow the guide here: [jvpernis/esp32-ps3](https://github.com/jvpernis/esp32-ps3)

##Memory limitations
Due to the limited memory on the esp32 module, it cannot run wifi and bluetooth at the same time. If you use a PS3 controller, enable it in `secrets.h` - it will automatically disable the web server.

References:
- https://www.instructables.com/DIY-ESP32-Wifi-Self-Balancing-Robot-B-Robot-ESP32-/
- https://www.pcbway.com/project/shareproject/ESP32_Balancing_Robot_Shield.html
- https://www.thingiverse.com/thing:2306541
- https://github.com/bobboteck/JoyStick
- https://github.com/madhephaestus/ESP32Servo
- https://github.com/jvpernis/esp32-ps3

Troubleshooting:
- https://github.com/arduino/arduino-ide/issues/1030
    - rm -rf ~/Library/Application\ Support/arduino-ide/

TODO:
- evaluate this library to support more controllers: https://bluepad32.readthedocs.io/en/latest/supported_gamepads/