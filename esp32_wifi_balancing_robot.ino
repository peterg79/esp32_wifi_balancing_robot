/*
   -- Esp32 Robot --

   This source code of graphical user interface
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;

   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/

#include <Wire.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
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

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__WIFI

#include <WiFi.h>

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID WIFI_SSID
#define REMOTEXY_WIFI_PASSWORD WIFI_PASS
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_ACCESS_PASSWORD "none"


#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 102 bytes
  { 255,5,0,0,0,95,0,19,0,0,0,69,83,80,51,50,32,82,111,98,
  111,116,0,24,2,106,200,200,84,1,1,4,0,10,41,8,25,25,88,8,
  24,24,48,4,26,24,79,78,0,31,79,70,70,0,5,11,102,85,85,126,
  10,60,60,32,213,26,31,1,25,39,57,57,20,28,24,24,0,213,31,83,
  101,114,118,111,0,1,25,105,57,57,87,45,24,24,4,24,31,240,159,148,
  138,0 };

// this structure defines all the variables and events of your control interface
struct {

    // input variables
  uint8_t pushSwitch_01; // =1 if state is ON, else =0
  int8_t joy_x; // from -100 to 100
  int8_t joy_y; // from -100 to 100
  uint8_t servo; // =1 if button pressed, else =0
  uint8_t buzzer; // =1 if button pressed, else =0

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_PUSHSWITCH_01 32

void initTimers();

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Booting");

  // TODO you setup code

  /// Robot init
  Serial.println();
  Serial.println(F("*ESP32 Self Balancing Robot*"));
  Serial.println(F("--------------------------------------------------------"));

  pinMode(PIN_ENABLE_MOTORS, OUTPUT);
  digitalWrite(PIN_ENABLE_MOTORS, HIGH);

  pinMode(PIN_MOTOR1_DIR, OUTPUT);
  pinMode(PIN_MOTOR1_STEP, OUTPUT);
  pinMode(PIN_MOTOR2_DIR, OUTPUT);
  pinMode(PIN_MOTOR2_STEP, OUTPUT);
  pinMode(PIN_SERVO, OUTPUT);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // led up as wifi is connected
  pinMode(PIN_WIFI_LED, OUTPUT);
  digitalWrite(PIN_WIFI_LED, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  ledcAttach(PIN_SERVO, 50, 16); // 50 Hz, 16-bit width
  delay(50);
  ledcWrite(PIN_SERVO, SERVO_AUX_NEUTRO);

  Serial.println(F("Wire.begin()"));
  Wire.begin();

  Serial.println(F("MPU6050_setup()"));
  MPU6050_setup();
  delay(500);
  Serial.println(F("MPU6050_calibrate()"));
  MPU6050_calibrate();

  initTimers();

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

  /// Robot init end
  Serial.println(F("RemoteXY_Init ()"));
  RemoteXY_Init ();
  pinMode (PIN_PUSHSWITCH_01, OUTPUT);

  /// Arduino OTA
  Serial.println(F("ArduinoOTA.begin()"));
  ArduinoOTA.begin();
  /// Arduino OTA End

  // TODO: web server
  Serial.println(F("Setup complete"));
}

void loop()
{
  //Serial.println(F("Loop start"));
  //Serial.println(F("OTA"));
  ArduinoOTA.handle();
  //Serial.println(F("RemoteXY"));
  RemoteXY_Handler ();
  //Serial.println(F("LED"));
  digitalWrite(PIN_PUSHSWITCH_01, (RemoteXY.pushSwitch_01==0)?LOW:HIGH);

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()

  timer_value = micros();
  if (MPU6050_newData()) {
    MPU6050_read_3axis();
    dt = (timer_value - timer_old) * 0.000001; // dt in seconds
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
    actual_robot_speed = (speed_M1 + speed_M2) / 2; // Positive: forward

    int16_t angular_velocity = (angle_adjusted - angle_adjusted_Old) * 25.0; // 25 is an empirical extracted factor to adjust for real units
    int16_t estimated_speed = -actual_robot_speed + angular_velocity;
    estimated_speed_filtered = estimated_speed_filtered * 0.9 + (float) estimated_speed * 0.1; // low pass filter on estimated speed

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
    target_angle = constrain(target_angle, -max_target_angle, max_target_angle); // limited output

    // Stability control (100Hz loop): This is a PD controller.
    //    input: robot target angle(from SPEED CONTROL), variable: robot angle, output: Motor speed
    //    We integrate the output (sumatory), so the output is really the motor acceleration, not motor speed.
    control_output += stabilityPDControl(dt, angle_adjusted, target_angle, Kp, Kd);
    control_output = constrain(control_output, -MAX_CONTROL_OUTPUT,  MAX_CONTROL_OUTPUT); // Limit max output from control

    // The steering part from the user is injected directly to the output
    motor1 = control_output + steering;
    motor2 = control_output - steering;

    // Limit max speed (control output)
    motor1 = constrain(motor1, -MAX_CONTROL_OUTPUT, MAX_CONTROL_OUTPUT);
    motor2 = constrain(motor2, -MAX_CONTROL_OUTPUT, MAX_CONTROL_OUTPUT);

    int angle_ready;
    if (RemoteXY.servo == 1)     // If we press the SERVO button we start to move
      angle_ready = 82;
    else
      angle_ready = 74;  // Default angle

    if ((angle_adjusted < angle_ready) && (angle_adjusted > -angle_ready)) // Is robot ready (upright?)
    {
      // NORMAL MODE
      digitalWrite(PIN_ENABLE_MOTORS, LOW);  // Motors enable
      // NOW we send the commands to the motors
      setMotorSpeedM1(motor1);
      setMotorSpeedM2(motor2);
    } else   // Robot not ready (flat), angle > angle_ready => ROBOT OFF
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
    if (RemoteXY.servo == 1) {
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
      Kp = Kp_user;            // Default user control gains
      Kd = Kd_user;
      Kp_thr = Kp_thr_user;
      Ki_thr = Ki_thr_user;
    } else // We are in the raise up procedure => we use special control parameters
    {
      Kp = KP_RAISEUP;         // CONTROL GAINS FOR RAISE UP
      Kd = KD_RAISEUP;
      Kp_thr = KP_THROTTLE_RAISEUP;
      Ki_thr = KI_THROTTLE_RAISEUP;
    }
  }

  static int sound_mode = 0;
  uint32_t sound_start = 0;
  if(sound_mode == 0 && RemoteXY.buzzer) {
    sound_mode = 1;
    sound_start = millis();
    digitalWrite(PIN_BUZZER, HIGH);
  }
  uint32_t now = millis();
  switch(sound_mode) {
    case 1:
      if (now >= sound_start + 150) {
        sound_mode = 2;
        digitalWrite(PIN_BUZZER, LOW);
      }
    case 2:
      if (now >= sound_start + 150 + 80) {
        sound_mode = 3;
        digitalWrite(PIN_BUZZER, HIGH);
      }
    case 3:
      if (now >= sound_start + 150 + 80 + 150) {
        sound_mode = 0;
        digitalWrite(PIN_BUZZER, LOW);
      }
  }

  //Serial.println(F("Loop end"));
}
