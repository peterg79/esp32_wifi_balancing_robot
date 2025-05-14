# ESP32_Wifi_Balancing_Robot
 An Self Balancing Robot based ESP32 can be controlled use Android over Wifi

This self-balancing robot is based on [B-Robot][https://www.jjrobots.com/much-more-than-a-self-balancing-robot/], which is open sourced on jjrobots. Thanks to the article by "[ghmartin77][https://www.jjrobots.com/community/migrated-forums-4-jjrobots-b-robot/esp32-port-of-b-robot_evo2-code-3/]" posted on the B-Robot forum.

This repository is a fork of [bluino/esp32_wifi_balancing_robot](https://github.com/bluino/esp32_wifi_balancing_robot)

To compile, install [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer) and [AsyncTCP](https://github.com/ESP32Async/AsyncTCP) libraries.
Then, copy [secret-template.h](secret-template.h) to `secret.h` and edit credentials there.
Generate `control.txt` using the following command:
`sed -e 's,\",\\\",g' control.html | awk '{print "\""$0"\\n\""}' > control.txt`

Then you're ready to compile and upload the firmware.

TODO:
- https://ai.thestempedia.com/docs/dabble-app/gamepad-module/
- https://remotexy.com/en/examples/


## Communication protocol
  input messages:
    - {"verbose": 0|1} : default 0. if 0, only necessary status is communicated. Otherwise, debug info too.
    - {"buzzer": true|false} : default false. Sounds buzzer.
    - {"light": true|false} : default no change. Switch light on.
    - {"servo": true|false} : default no change. Switch servo on.
    //- {"speed":0-100,"angle":0-359} : default no change. move motors to direction. not yet implemented.
    - {"x":-100 - 100,"y":-100 - 100} : default no change. move motors to direction.
    example:
      {"verbose": 1, "light": true, "servo": true, "x": 75,"y": 0}
  output messages:
    - {"buzzer": true|false}
    - {"light": true|false}
    - {"servo": true|false}
    - {"angle": -180.0 - 180.0}
    - {"debug": {"angle_adjusted": 91.1, "angle_adjusted_filtered": 1.01}}
