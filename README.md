# ESP32_Wifi_Balancing_Robot
 An Self Balancing Robot based ESP32 can be controlled use Android over Wifi

This self-balancing robot is based on [B-Robot][https://www.jjrobots.com/much-more-than-a-self-balancing-robot/], which is open sourced on jjrobots. Thanks to the article by "[ghmartin77][https://www.jjrobots.com/community/migrated-forums-4-jjrobots-b-robot/esp32-port-of-b-robot_evo2-code-3/]" posted on the B-Robot forum.

This repository is a fork of [bluino/esp32_wifi_balancing_robot](https://github.com/bluino/esp32_wifi_balancing_robot)

To compile, install [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer) and [AsyncTCP](https://github.com/ESP32Async/AsyncTCP) libraries.
Then, copy [secret-template.h](secret-template.h) to `secret.h` and edit credentials there.
Install [littlefs-uploader](https://github.com/earlephilhower/arduino-littlefs-upload) and upload static content (`index.html` and `index.css`) to the ESP32.

Then you're ready to compile and upload the firmware.

References:
- https://www.instructables.com/DIY-ESP32-Wifi-Self-Balancing-Robot-B-Robot-ESP32-/
- https://www.pcbway.com/project/shareproject/ESP32_Balancing_Robot_Shield.html
- https://www.thingiverse.com/thing:2306541
- https://github.com/bobboteck/JoyStick
- https://github.com/madhephaestus/ESP32Servo
