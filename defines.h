/*
 * defines.h
 *
 *  Created on: 25.09.2017
 *      Author: anonymous
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#define PIN_ENABLE_MOTORS 12
#define PIN_MOTOR1_DIR 27
#define PIN_MOTOR1_STEP 14
#define PIN_MOTOR2_DIR 25
#define PIN_MOTOR2_STEP 26
#define PIN_SERVO 13
#define PIN_LED 32
#define PIN_WIFI_LED 2
#define PIN_BUZZER 33

// NORMAL MODE PARAMETERS (MAXIMUN SETTINGS)
#define MAX_THROTTLE 550
#define MAX_STEERING 140
#define MAX_TARGET_ANGLE 14

// PRO MODE = MORE AGGRESSIVE (MAXIMUN SETTINGS)
#define MAX_THROTTLE_PRO 780     // Max recommended value: 860
#define MAX_STEERING_PRO 260     // Max recommended value: 280
#define MAX_TARGET_ANGLE_PRO 26  // Max recommended value: 32

// Default control terms for EVO 2
#define KP 0.32
#define KD 0.050
#define KP_THROTTLE 0.080
#define KI_THROTTLE 0.1
#define KP_POSITION 0.06
#define KD_POSITION 0.45
//#define KI_POSITION 0.02

// Control gains for raiseup (the raiseup movement requiere special control parameters)
#define KP_RAISEUP 0.1
#define KD_RAISEUP 0.16
#define KP_THROTTLE_RAISEUP 0  // No speed control on raiseup
#define KI_THROTTLE_RAISEUP 0.0

#define MAX_CONTROL_OUTPUT 500
#define ITERM_MAX_ERROR 30  // Iterm windup constants for PI control
#define ITERM_MAX 10000

#define ANGLE_OFFSET 0.0  // Offset angle for balance (to compensate robot own weight distribution)

// Servo definitions
#define SERVO_FREQUENCY 50
#define SERVO_TIMER_WIDTH 16
#define SERVO_MIN_PULSE_WIDTH 400
#define SERVO_MAX_PULSE_WIDTH 2600

#define SERVO2_NEUTRO 4444
#define SERVO2_RANGE 8400

#define ZERO_SPEED 0xffffff
#define MAX_ACCEL 14  // Maximun motor acceleration (MAX RECOMMENDED VALUE: 20) (default:14)

#define MICROSTEPPING 8  // 8 or 16 for 1/8 or 1/16 driver microstepping (default:16)

// AUX definitions
#define CLR(x, y) (x &= (~(1 << y)))
#define SET(x, y) (x |= (1 << y))
#define RAD2GRAD 57.2957795
#define GRAD2RAD 0.01745329251994329576923690768489

#endif /* DEFINES_H_ */
