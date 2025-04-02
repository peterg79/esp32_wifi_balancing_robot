#include "index.h"

const char *index_html PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>Esp32 Robot Remote</title>
    <link rel="stylesheet" href="index.css" />
    <script>
      // buttons from https://www.toptal.com/designers/htmlarrows/arrows/

      /*
      alternative projects:
        - https://www.instructables.com/Making-a-Joystick-With-HTML-pure-JavaScript/
        - https://www.reddit.com/r/sveltejs/comments/w0gs0i/project_foss_turn_your_mobile_device_into_a/
        - https://parvaiz.pk/posts/virtual-joystick-for-arduino/
        - ! https://www.instructables.com/Making-a-Joystick-With-HTML-pure-JavaScript/
      */

      var gateway = `ws://${window.location.hostname}/ws`;
      if (window.location.protocol.startsWith("file")) {
        gateway = "ws://172.27.201.232/ws";
      }
      var websocket;

      function initWebSocket() {
        console.log("Trying to open a WebSocket connection...");
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
        websocket.onmessage = onMessage;
      }

      function onOpen(event) {
        console.log("Connection opened");
        websocket.send("status");
      }

      function onClose(event) {
        console.log("Connection closed");
        setTimeout(initWebSocket, 2000);
      }

      // https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/Writing_WebSocket_client_applications
      function onMessage(event) {
        log("received " + event.data);
        const msg = JSON.parse(event.data);
        // sample response:
        // {"light": true, "pro": false}
        for (var key in msg) {
          value = msg[key];
          log("processing '" + key + "': '" + value + "'");
          switch (key) {
            case "light":
              document.getElementById("light").checked = value;
              break;
            case "pro":
              document.getElementById("pro").checked = value;
              if (value) {
                labelContent = "&#128012;";
                document.getElementById("prolabel").innerHTML = labelContent;
                log("pro label changed to " + labelContent);
              } else {
                labelContent = "&#127939;";
                document.getElementById("prolabel").innerHTML = labelContent;
                log("pro label changed to " + labelContent);
              }
              break;
          }
        }
      }

      window.addEventListener("load", onLoad);

      function onLoad(event) {
        initWebSocket();
        initLight();
        initPro();
        initServo();
        initHorn();
        initJoystick();
      }

      function initLight() {
        document.getElementById("light").addEventListener("click", toggleLight);
        log("light initialized.");
      }

      function toggleLight() {
        log("toggle light");
        websocket.send("toggle_light");
      }

      function initPro() {
        document.getElementById("pro").addEventListener("click", togglePro);
        log("pro initialized.");
      }

      function togglePro() {
        log("toggle pro");
        websocket.send("toggle_pro");
      }

      function log(msg) {
        const container = document.getElementById("log");
        container.textContent = `${msg} \n${container.textContent}`;
      }

      function stopServo(evt) {
        evt.preventDefault();
        log("stop servo");
        websocket.send("stop_servo");
      }

      function startServo(evt) {
        evt.preventDefault();
        log("start servo");
        websocket.send("start_servo");
      }

      function ignoreEvent(evt) {
        evt.preventDefault();
      }

      function initServo() {
        const servo = document.getElementById("servo");
        servo.addEventListener("touchstart", startServo, { passive: false });
        servo.addEventListener("touchend", stopServo, { passive: false });
        servo.addEventListener("touchmove", ignoreEvent, { passive: false });
        servo.addEventListener("touchcancel", ignoreEvent, { passive: false });
        servo.addEventListener("mousedown", startServo, { passive: false });
        servo.addEventListener("mouseup", stopServo, { passive: false });
        log("servo initialized.");
      }

      function startHorn(evt) {
        evt.preventDefault();
        log("blow horn");
        websocket.send("horn");
      }

      function initHorn() {
        const horn = document.getElementById("horn");
        horn.addEventListener("touchstart", startHorn, { passive: false });
        horn.addEventListener("touchend", ignoreEvent, { passive: false });
        horn.addEventListener("touchmove", ignoreEvent, { passive: false });
        horn.addEventListener("touchcancel", ignoreEvent, { passive: false });
        horn.addEventListener("mousedown", startHorn, { passive: false });
        horn.addEventListener("mouseup", ignoreEvent, { passive: false });
        log("horn initialized.");
      }

      function goUp(evt) {
        evt.preventDefault();
        log("go up");
        websocket.send("up");
      }
      function stopUp(evt) {
        evt.preventDefault();
        log("stop up");
        websocket.send("stop_up");
      }

      function goDown(evt) {
        evt.preventDefault();
        log("go down");
        websocket.send("down");
      }
      function stopDown(evt) {
        evt.preventDefault();
        log("stop down");
        websocket.send("stop_down");
      }

      function goLeft(evt) {
        evt.preventDefault();
        log("go left");
        websocket.send("left");
      }
      function stopLeft(evt) {
        evt.preventDefault();
        log("stop left");
        websocket.send("stop_left");
      }

      function goRight(evt) {
        evt.preventDefault();
        log("go right");
        websocket.send("right");
      }
      function stopRight(evt) {
        evt.preventDefault();
        log("stop right");
        websocket.send("stop_right");
      }

      function initJoystick() {
        const up = document.getElementById("up");
        const down = document.getElementById("down");
        const left = document.getElementById("left");
        const right = document.getElementById("right");

        up.addEventListener("touchstart", goUp, { passive: false });
        up.addEventListener("touchend", stopUp, { passive: false });
        up.addEventListener("touchmove", ignoreEvent, { passive: false });
        up.addEventListener("touchcancel", ignoreEvent, { passive: false });
        up.addEventListener("mousedown", goUp, { passive: false });
        up.addEventListener("mouseup", stopUp, { passive: false });

        down.addEventListener("touchstart", goDown, { passive: false });
        down.addEventListener("touchend", stopDown, { passive: false });
        down.addEventListener("touchmove", ignoreEvent, { passive: false });
        down.addEventListener("touchcancel", ignoreEvent, { passive: false });
        down.addEventListener("mousedown", goDown, { passive: false });
        down.addEventListener("mouseup", stopDown, { passive: false });

        left.addEventListener("touchstart", goLeft, { passive: false });
        left.addEventListener("touchend", stopLeft, { passive: false });
        left.addEventListener("touchmove", ignoreEvent, { passive: false });
        left.addEventListener("touchcancel", ignoreEvent, { passive: false });
        left.addEventListener("mousedown", goLeft, { passive: false });
        left.addEventListener("mouseup", stopLeft, { passive: false });

        right.addEventListener("touchstart", goRight, { passive: false });
        right.addEventListener("touchend", stopRight, { passive: false });
        right.addEventListener("touchmove", ignoreEvent, { passive: false });
        right.addEventListener("touchcancel", ignoreEvent, { passive: false });
        right.addEventListener("mousedown", goRight, { passive: false });
        right.addEventListener("mouseup", stopRight, { passive: false });

        log("joystick initialized.");
      }
    </script>
  </head>
  <body>
    <table class="tg">
      <thead>
        <tr>
          <td>
            <button id="up" class="button buttonUpDown">&uarr;</button><br />
            <button id="down" class="button buttonUpDown">&darr;</button>
          </td>
          <td>
            <button id="servo" class="button buttonServo">&orarr;</button>
            <br />
            <button id="horn" class="button buttonHorn">&#9836;</button>
            <input type="checkbox" class="hidden" id="light" %LIGHT_IS_ON% />
            <label for="light" class="button buttonLight">&#9967;</label>
            <input type="checkbox" class="hidden" id="pro" %PRO_MODE% />
            <label for="pro" id="prolabel" class="button buttonPro"
              >%PRO_MODE_HTML%</label
            >
          </td>
          <td>
            <span style="white-space: nowrap">
              <button id="left" class="button buttonLeftRight">&larr;</button>
              <button id="right" class="button buttonLeftRight">&rarr;</button>
            </span>
          </td>
        </tr>
      </thead>
    </table>
    <pre id="log" style="border: 1px solid #ccc"></pre>
  </body>
</html>
)rawliteral";

const char *index_css PROGMEM = R"rawliteral(
body {
  user-select: none;
}
.tg {
  border-collapse: collapse;
  border-spacing: 0;
}
.tg td {
  border-color: black;
  border-style: solid;
  border-width: 0px;
  font-family: Arial, sans-serif;
  font-size: 14px;
  overflow: hidden;
  padding: 10px 5px;
  word-break: normal;
  width: 33%;
  text-align: center;
  vertical-align: center;
}
.tg th {
  border-color: black;
  border-style: solid;
  border-width: 1px;
  font-family: Arial, sans-serif;
  font-size: 14px;
  font-weight: normal;
  overflow: hidden;
  padding: 10px 5px;
  word-break: normal;
}
.tg .tg-0lax {
  text-align: left;
  vertical-align: top;
}

.button {
  background-color: #04aa6d; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  margin: 4px 2px;
  cursor: pointer;
}

.buttonLeftRight {
  font-size: 32px;
  height: 200px;
}
.buttonUpDown {
  font-size: 24px;
  height: 100px;
  width: 100%;
}
.buttonServo {
  font-size: 64px;
  height: 100px;
  width: 100%;
}
.buttonHorn {
  font-size: 18px;
}
.buttonLight {
  font-size: 24px;
}
.buttonPro {
  font-size: 24px;
}
.toggle {
  appearance: none;
  position: relative;
  display: inline-block;
  box-sizing: content-box;
  width: 4.5em;
  height: 2em;
  padding: 0.2em;
  border: none;
  cursor: pointer;
  border-radius: 1.5em;
  overflow: hidden;
  background-color: #707070;
  transition: background ease 0.3s;
}

.toggle:before {
  content: "on off";
  display: block;
  position: absolute;
  z-index: 2;
  width: 2em;
  height: 2em;
  font-family: system-ui;
  font-size: 1em;
  line-height: 2em;
  font-weight: 500;
  text-transform: uppercase;
  text-indent: -2em;
  word-spacing: 2.55em;
  text-shadow: -1px -1px rgba(0, 0, 0, 0.15);
  white-space: nowrap;
  background: #fff;
  color: #fff;
  border-radius: 1.5em;
  transition: transform cubic-bezier(0.3, 1.5, 0.7, 1) 0.3s;
}

.toggle:checked {
  background-color: #4cd964;
}

.toggle:checked:before {
  transform: translateX(2.5em);
}

.hidden {
  position: absolute;
  visibility: hidden;
  opacity: 0;
}

input[type="checkbox"] + label {
  font-style: normal;
  border: none;
}

input[type="checkbox"]:checked + label {
  color: #e0e310;
  font-style: normal;
  border: none;
}

#log {
  height: 200px;
  width: 600px;
  overflow: scroll;
}
)rawliteral";
