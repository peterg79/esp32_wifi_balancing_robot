/*
 * esp32_wifi_balancing_robot.ino
 *
 *  Created on: 23.02.2021
 *      Author: anonymous
 */

#include <Wire.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <LittleFS.h>
#include <Preferences.h>
#include "Control.h"
#include "MPU6050.h"
#include "Motors.h"
#include "defines.h"
#include "globals.h"
#include <stdio.h>
#include "esp_types.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "driver/ledc.h"
#include "esp32-hal-ledc.h"
#include "secret.h"

#define FORMAT_LITTLEFS_IF_FAILED true

const char* PARAM_FADER1 = "fader1";
const char* PARAM_FADER2 = "fader2";
const char* PARAM_PUSH1 = "push1";
const char* PARAM_PUSH2 = "push2";
const char* PARAM_PUSH3 = "push3";
const char* PARAM_PUSH4 = "push4";
const char* PARAM_TOGGLE1 = "toggle1";
const char* PARAM_FADER3 = "fader3";
const char* PARAM_FADER4 = "fader4";
const char* PARAM_FADER5 = "fader5";
const char* PARAM_FADER6 = "fader6";

Preferences preferences;

unsigned long previousMillis = 0;

AsyncWebServer server(80);
DNSServer dnsServer;

unsigned long lastCommandTime;

void initMPU6050() {
  MPU6050_setup();
  delay(500);
  MPU6050_calibrate();
}

void initTimers();

// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

// list all of the files, if ishtml=true, return html rather than simple text
String listFiles(bool ishtml) {
  String returnText = "";
  Serial.println("Listing files stored on SPIFFS");
  File root = LittleFS.open("/");
  File foundfile = root.openNextFile();
  if (ishtml) {
    returnText += "<table><tr><th align='left'>Name</th><th align='left'>Size</th></tr>";
  }
  while (foundfile) {
    if (ishtml) {
      returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td></tr>";
    } else {
      returnText += "File: " + String(foundfile.name()) + "\n";
    }
    foundfile = root.openNextFile();
  }
  if (ishtml) {
    returnText += "</table>";
  }
  root.close();
  foundfile.close();
  return returnText;
}

String processor(const String& var) {
  if (var == "FILELIST") {
    return listFiles(true);
  }
  if (var == "FREEFS") {
    return humanReadableSize((LittleFS.totalBytes() - LittleFS.usedBytes()));
  }

  if (var == "USEDFS") {
    return humanReadableSize(LittleFS.usedBytes());
  }

  if (var == "TOTALFS") {
    return humanReadableSize(LittleFS.totalBytes());
  }

  if (var == "WIFI_MODE") {
    if (preferences.getBool("wifi_sta", true))
      return "STA";
    else
      return "AP";
  }

  return String();
}

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

// handles uploads
void handleUpload(AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  Serial.println(logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object
    request->_tempFile = LittleFS.open("/" + filename, "w");
    Serial.println(logmessage);
  }

  if (len) {
    // stream the incoming chunk to the opened file
    request->_tempFile.write(data, len);
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    Serial.println(logmessage);
  }

  if (final) {
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    // close the file handle as the upload is now done
    request->_tempFile.close();
    Serial.println(logmessage);
    request->redirect("/");
  }
}

bool filterHtml(AsyncWebServerRequest* request) {
  return request->url().endsWith("html") || request->url().endsWith("/");
}

void setup() {
  Serial.begin(115200);  // set up seriamonitor at 115200 bps
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println("*ESP32 Camera Balancing Robot*");
  Serial.println("--------------------------------------------------------");


  pinMode(PIN_ENABLE_MOTORS, OUTPUT);
  digitalWrite(PIN_ENABLE_MOTORS, HIGH);

  pinMode(PIN_MOTOR1_DIR, OUTPUT);
  pinMode(PIN_MOTOR1_STEP, OUTPUT);
  pinMode(PIN_MOTOR2_DIR, OUTPUT);
  pinMode(PIN_MOTOR2_STEP, OUTPUT);
  pinMode(PIN_SERVO, OUTPUT);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_WIFI_LED, OUTPUT);
  digitalWrite(PIN_WIFI_LED, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  ledcAttach(PIN_SERVO, 50, 16);  // 50 Hz, 16-bit width
  delay(50);
  ledcWrite(PIN_SERVO, SERVO_AUX_NEUTRO);

  Wire.begin();
  initMPU6050();

  preferences.begin("robot", false);

  // Set NodeMCU Wifi hostname based on chip mac address
  char chip_id[15];
  snprintf(chip_id, 15, "%04X", (uint16_t)(ESP.getEfuseMac() >> 32));
  String hostname = "esp32brobot-" + String(chip_id);

  Serial.println();
  Serial.println("Hostname: " + hostname);
  IPAddress myIP;

  if (preferences.getBool("wifi_sta", true)) {
    // first, set NodeMCU as STA mode to connect with a Wifi network
    WiFi.mode(WIFI_STA);
    WiFi.hostname(hostname);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("");
    Serial.print("Connecting to: ");
    Serial.println(WIFI_SSID);
    Serial.print("Password: ");
    Serial.println(WIFI_PASS);

    // try to connect with Wifi network about 8 seconds
    unsigned long currentMillis = millis();
    previousMillis = currentMillis;
    while (WiFi.status() != WL_CONNECTED && currentMillis - previousMillis <= 8000) {
      delay(500);
      Serial.print(".");
      currentMillis = millis();
    }

    // if failed to connect with Wifi network set NodeMCU as AP mode
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("*WiFi-STA-Mode*");
      Serial.print("IP: ");
      myIP = WiFi.localIP();
      Serial.println(myIP);
      digitalWrite(PIN_WIFI_LED, HIGH);  // Wifi LED on when connected to Wifi as STA mode
      delay(2000);
    } else {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(hostname.c_str());
      myIP = WiFi.softAPIP();
      Serial.println("");
      Serial.println("WiFi failed connected to " + String(WIFI_SSID));
      Serial.println("");
      Serial.println("*WiFi-AP-Mode*");
      Serial.print("AP IP address: ");
      Serial.println(myIP);
      digitalWrite(PIN_WIFI_LED, LOW);  // Wifi LED off when status as AP mode
      delay(2000);
    }
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hostname.c_str());
    myIP = WiFi.softAPIP();
    Serial.println("");
    Serial.println("*WiFi-AP-Mode*");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    digitalWrite(PIN_WIFI_LED, LOW);  // Wifi LED off when status as AP mode
    delay(2000);
  }

  Serial.println("Starting DNS Server");
  // redirecting any domain name (*) to the IP of ESP32
  dnsServer.start(53, "*", WiFi.softAPIP());

  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS Mount Failed");
  }

  // Send a GET request to <ESP_IP>/?fader=<inputValue>
  server.on("/robot", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputValue;
    String inputMessage;
    OSCnewMessage = 1;
    lastCommandTime = millis();

    // Get value for Forward/Backward
    if (request->hasParam(PARAM_FADER1) || request->hasParam(PARAM_FADER2)) {
      if (request->hasParam(PARAM_FADER1)) {
        OSCpage = 1;
        inputValue = request->getParam(PARAM_FADER1)->value();
        inputMessage = PARAM_FADER1;
        OSCfader[0] = inputValue.toFloat();
      }
      // Get value for Right/Left
      if (request->hasParam(PARAM_FADER2)) {
        OSCpage = 1;
        inputValue = request->getParam(PARAM_FADER2)->value();
        inputMessage = PARAM_FADER2;
        OSCfader[1] = inputValue.toFloat();
      }
    }
    // Get value for Servo0
    else if (request->hasParam(PARAM_PUSH1)) {
      OSCpage = 1;
      inputValue = request->getParam(PARAM_PUSH1)->value();
      inputMessage = PARAM_PUSH1;
      if (inputValue.equals("1")) OSCpush[0] = 1;
      else OSCpush[0] = 0;
    }
    // Get value for Setting
    else if (request->hasParam(PARAM_PUSH2)) {
      OSCpage = 2;
      inputValue = request->getParam(PARAM_PUSH2)->value();
      inputMessage = PARAM_PUSH2;
      if (inputValue.equals("1")) OSCpush[2] = 1;
      else OSCpush[2] = 0;
    }
    // Get value for Buzzer
    else if (request->hasParam(PARAM_PUSH3)) {
      inputValue = request->getParam(PARAM_PUSH3)->value();
      inputMessage = PARAM_PUSH3;
      if (inputValue.equals("1")) {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(150);
        digitalWrite(PIN_BUZZER, LOW);
        delay(80);
        digitalWrite(PIN_BUZZER, HIGH);
        delay(150);
        digitalWrite(PIN_BUZZER, LOW);
        delay(80);
      }
    }
    // Get value for Led
    else if (request->hasParam(PARAM_PUSH4)) {
      inputValue = request->getParam(PARAM_PUSH4)->value();
      inputMessage = PARAM_PUSH4;
      if (inputValue.equals("1")) digitalWrite(PIN_LED, HIGH);
      else digitalWrite(PIN_LED, LOW);
    }
    // Get value for mode PRO
    else if (request->hasParam(PARAM_TOGGLE1)) {
      OSCpage = 1;
      inputValue = request->getParam(PARAM_TOGGLE1)->value();
      inputMessage = PARAM_TOGGLE1;
      if (inputValue.equals("1")) OSCtoggle[0] = 1;
      else OSCtoggle[0] = 0;
    }
    // Get value for P-Stability
    else if (request->hasParam(PARAM_FADER3)) {
      OSCpage = 2;
      inputValue = request->getParam(PARAM_FADER3)->value();
      inputMessage = PARAM_FADER3;
      OSCfader[0] = inputValue.toFloat();
    }
    // Get value for D-Stability
    else if (request->hasParam(PARAM_FADER4)) {
      OSCpage = 2;
      inputValue = request->getParam(PARAM_FADER4)->value();
      inputMessage = PARAM_FADER4;
      OSCfader[0] = inputValue.toFloat();
    }
    // Get value for P-Speed
    else if (request->hasParam(PARAM_FADER5)) {
      OSCpage = 2;
      inputValue = request->getParam(PARAM_FADER5)->value();
      inputMessage = PARAM_FADER5;
      OSCfader[0] = inputValue.toFloat();
    }
    // Get value for I-Speed
    else if (request->hasParam(PARAM_FADER6)) {
      OSCpage = 2;
      inputValue = request->getParam(PARAM_FADER6)->value();
      inputMessage = PARAM_FADER6;
      OSCfader[0] = inputValue.toFloat();
    } else {
      inputValue = "No message sent";
    }
    Serial.println(inputMessage + '=' + inputValue);

    char rsp[256];
    sprintf(rsp, "{\"angle_adjusted\": %.2f}", angle_adjusted);
    request->send(200, "text/json", rsp);
  });

  server.on(
    "/upload", HTTP_POST, [](AsyncWebServerRequest* request) {
      request->send(200);
    },
    handleUpload);

  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam("mode")) {
      const String& in = request->getParam("mode")->value();
      preferences.putBool("wifi_sta", in == "sta");
    }
    if (preferences.getBool("wifi_sta", true)) {
      request->send(200, "text/plain", "STA");
    } else {
      request->send(200, "text/plain", "AP");
    }
  });

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest* request) {
    ESP.restart();
  });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html").setCacheControl("max-age=15").setTemplateProcessor(processor).setFilter(filterHtml);
  server.serveStatic("/", LittleFS, "/").setCacheControl("max-age=15");

  server.onNotFound(notFound);  // when a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();               // actually start the server

  initTimers();

  // default neutral values
  OSCfader[0] = 0.5;
  OSCfader[1] = 0.5;
  OSCfader[2] = 0.5;
  OSCfader[3] = 0.5;

  digitalWrite(PIN_ENABLE_MOTORS, LOW);
  for (uint8_t k = 0; k < 5; k++) {
    setMotorSpeedM1(5);
    setMotorSpeedM2(5);
    ledcWrite(PIN_SERVO, SERVO_AUX_NEUTRO + 250);
    delay(200);
    setMotorSpeedM1(-5);
    setMotorSpeedM2(-5);
    ledcWrite(PIN_SERVO, SERVO_AUX_NEUTRO - 250);
    delay(200);
  }
  ledcWrite(PIN_SERVO, SERVO_AUX_NEUTRO);

  ArduinoOTA.begin();  // enable to receive update/upload firmware via Wifi OTA
  lastCommandTime = millis();
}

void loop() {
  if (millis() > lastCommandTime + 1000 * 60 * 10) {
    // deep sleep after 10 minues of inactivity
    setMotorSpeedM1(0);
    setMotorSpeedM2(0);
    digitalWrite(PIN_ENABLE_MOTORS, HIGH);  // Disable motors
    esp_deep_sleep_start();
  }

  ArduinoOTA.handle();

  if (OSCnewMessage) {
    OSCnewMessage = 0;
    processOSCMsg();
  }

  timer_value = micros();

  if (MPU6050_newData()) {

    MPU6050_read_3axis();

    dt = (timer_value - timer_old) * 0.000001;  // dt in seconds
    //Serial.println(timer_value - timer_old);
    timer_old = timer_value;

    angle_adjusted_Old = angle_adjusted;
    // Get new orientation angle from IMU (MPU6050)
    float MPU_sensor_angle = MPU6050_getAngle(dt);
    angle_adjusted = MPU_sensor_angle + angle_offset;
    if ((MPU_sensor_angle > -15) && (MPU_sensor_angle < 15))
      angle_adjusted_filtered = angle_adjusted_filtered * 0.99 + MPU_sensor_angle * 0.01;


    // We calculate the estimated robot speed:
    // Estimated_Speed = angular_velocity_of_stepper_motors(combined) - angular_velocity_of_robot(angle measured by IMU)
    actual_robot_speed = (speed_M1 + speed_M2) / 2;  // Positive: forward

    int16_t angular_velocity = (angle_adjusted - angle_adjusted_Old) * 25.0;  // 25 is an empirical extracted factor to adjust for real units
    int16_t estimated_speed = -actual_robot_speed + angular_velocity;
    estimated_speed_filtered = estimated_speed_filtered * 0.9 + (float)estimated_speed * 0.1;  // low pass filter on estimated speed


    if (positionControlMode) {
      // POSITION CONTROL. INPUT: Target steps for each motor. Output: motors speed
      motor1_control = positionPDControl(steps1, target_steps1, Kp_position, Kd_position, speed_M1);
      motor2_control = positionPDControl(steps2, target_steps2, Kp_position, Kd_position, speed_M2);

      // Convert from motor position control to throttle / steering commands
      throttle = (motor1_control + motor2_control) / 2;
      throttle = constrain(throttle, -190, 190);
      steering = motor2_control - motor1_control;
      steering = constrain(steering, -50, 50);
    }

    // ROBOT SPEED CONTROL: This is a PI controller.
    //    input:user throttle(robot speed), variable: estimated robot speed, output: target robot angle to get the desired speed
    target_angle = speedPIControl(dt, estimated_speed_filtered, throttle, Kp_thr, Ki_thr);
    target_angle = constrain(target_angle, -max_target_angle, max_target_angle);  // limited output

    // Stability control (100Hz loop): This is a PD controller.
    //    input: robot target angle(from SPEED CONTROL), variable: robot angle, output: Motor speed
    //    We integrate the output (sumatory), so the output is really the motor acceleration, not motor speed.
    control_output += stabilityPDControl(dt, angle_adjusted, target_angle, Kp, Kd);
    control_output = constrain(control_output, -MAX_CONTROL_OUTPUT, MAX_CONTROL_OUTPUT);  // Limit max output from control

    // The steering part from the user is injected directly to the output
    motor1 = control_output + steering;
    motor2 = control_output - steering;

    // Limit max speed (control output)
    motor1 = constrain(motor1, -MAX_CONTROL_OUTPUT, MAX_CONTROL_OUTPUT);
    motor2 = constrain(motor2, -MAX_CONTROL_OUTPUT, MAX_CONTROL_OUTPUT);

    int angle_ready;
    if (OSCpush[0])  // If we press the SERVO button we start to move
      angle_ready = 82;
    else
      angle_ready = 74;                                                     // Default angle
    if ((angle_adjusted < angle_ready) && (angle_adjusted > -angle_ready))  // Is robot ready (upright?)
    {
      // NORMAL MODE
      digitalWrite(PIN_ENABLE_MOTORS, LOW);  // Motors enable
      // NOW we send the commands to the motors
      setMotorSpeedM1(motor1);
      setMotorSpeedM2(motor2);
    } else  // Robot not ready (flat), angle > angle_ready => ROBOT OFF
    {
      digitalWrite(PIN_ENABLE_MOTORS, HIGH);  // Disable motors
      setMotorSpeedM1(0);
      setMotorSpeedM2(0);
      PID_errorSum = 0;  // Reset PID I term
      Kp = KP_RAISEUP;   // CONTROL GAINS FOR RAISE UP
      Kd = KD_RAISEUP;
      Kp_thr = KP_THROTTLE_RAISEUP;
      Ki_thr = KI_THROTTLE_RAISEUP;
      // RESET steps
      steps1 = 0;
      steps2 = 0;
      positionControlMode = false;
      OSCmove_mode = false;
      throttle = 0;
      steering = 0;
    }

    // Push1 Move servo arm
    if (OSCpush[0]) {
      if (angle_adjusted > -40)
        ledcWrite(PIN_SERVO, SERVO_MAX_PULSEWIDTH);
      else
        ledcWrite(PIN_SERVO, SERVO_MIN_PULSEWIDTH);
    } else
      ledcWrite(PIN_SERVO, SERVO_AUX_NEUTRO);

    // Servo2
    //ledcWrite(PIN_SERVO, SERVO2_NEUTRO + (OSCfader[2] - 0.5) * SERVO2_RANGE);

    // Normal condition?
    if ((angle_adjusted < 56) && (angle_adjusted > -56)) {
      Kp = Kp_user;  // Default user control gains
      Kd = Kd_user;
      Kp_thr = Kp_thr_user;
      Ki_thr = Ki_thr_user;
    } else  // We are in the raise up procedure => we use special control parameters
    {
      Kp = KP_RAISEUP;  // CONTROL GAINS FOR RAISE UP
      Kd = KD_RAISEUP;
      Kp_thr = KP_THROTTLE_RAISEUP;
      Ki_thr = KI_THROTTLE_RAISEUP;
    }

  }  // End of new IMU data
}



void processOSCMsg() {
  if (OSCpage == 1) {
    if (modifing_control_parameters)  // We came from the settings screen
    {
      OSCfader[0] = 0.5;  // default neutral values
      OSCfader[1] = 0.5;  // default neutral values
      OSCtoggle[0] = 0;   // Normal mode
      mode = 0;
      modifing_control_parameters = false;
    }

    if (OSCmove_mode) {
      Serial.print("M ");
      Serial.print(OSCmove_speed);
      Serial.print(" ");
      Serial.print(OSCmove_steps1);
      Serial.print(",");
      Serial.println(OSCmove_steps2);
      positionControlMode = true;
      OSCmove_mode = false;
      target_steps1 = steps1 + OSCmove_steps1;
      target_steps2 = steps2 + OSCmove_steps2;
    } else {
      positionControlMode = false;
      throttle = (OSCfader[0] - 0.5) * max_throttle;
      // We add some exponential on steering to smooth the center band
      steering = OSCfader[1] - 0.5;
      if (steering > 0)
        steering = (steering * steering + 0.5 * steering) * max_steering;
      else
        steering = (-steering * steering + 0.5 * steering) * max_steering;
    }

    if ((mode == 0) && (OSCtoggle[0])) {
      // Change to PRO mode
      max_throttle = MAX_THROTTLE_PRO;
      max_steering = MAX_STEERING_PRO;
      max_target_angle = MAX_TARGET_ANGLE_PRO;
      mode = 1;
    }
    if ((mode == 1) && (OSCtoggle[0] == 0)) {
      // Change to NORMAL mode
      max_throttle = MAX_THROTTLE;
      max_steering = MAX_STEERING;
      max_target_angle = MAX_TARGET_ANGLE;
      mode = 0;
    }
  } else if (OSCpage == 2) {  // OSC page 2
    if (!modifing_control_parameters) {
      for (uint8_t i = 0; i < 4; i++)
        OSCfader[i] = 0.5;
      OSCtoggle[0] = 0;

      modifing_control_parameters = true;
      //OSC_MsgSend("$P2", 4);
    }
    // User could adjust KP, KD, KP_THROTTLE and KI_THROTTLE (fadder3,4,5,6)
    // Now we need to adjust all the parameters all the times because we dont know what parameter has been moved
    Kp_user = KP * 2 * OSCfader[0];
    Kd_user = KD * 2 * OSCfader[1];
    Kp_thr_user = KP_THROTTLE * 2 * OSCfader[2];
    Ki_thr_user = KI_THROTTLE * 2 * OSCfader[3];
    // Send a special telemetry message with the new parameters
    char auxS[50];
    sprintf(auxS, "$tP,%d,%d,%d,%d", int(Kp_user * 1000), int(Kd_user * 1000), int(Kp_thr_user * 1000), int(Ki_thr_user * 1000));
    //OSC_MsgSend(auxS, 50);


    // Calibration mode??
    if (OSCpush[2] == 1) {
      Serial.print("Calibration MODE ");
      angle_offset = angle_adjusted_filtered;
      Serial.println(angle_offset);
    }

    // Kill robot => Sleep
    while (OSCtoggle[0] == 1) {
      //Reset external parameters
      PID_errorSum = 0;
      timer_old = millis();
      setMotorSpeedM1(0);
      setMotorSpeedM2(0);
      digitalWrite(PIN_ENABLE_MOTORS, HIGH);  // Disable motors
    }
  }
}
