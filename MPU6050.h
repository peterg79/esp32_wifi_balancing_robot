/*
 * MPU6050.h
 *
 *  Created on: 22.09.2017
 *      Author: anonymous
 */

#include <Arduino.h>

#ifndef MPU6050_H_
#define MPU6050_H_

#define RAD2GRAD 57.2957795
#define GRAD2RAD 0.01745329251994329576923690768489

#define MPU6050_AUX_VDDIO 0x01           // R/W
#define MPU6050_SMPLRT_DIV 0x19          // R/W
#define MPU6050_CONFIG 0x1A              // R/W
#define MPU6050_GYRO_CONFIG 0x1B         // R/W
#define MPU6050_ACCEL_CONFIG 0x1C        // R/W
#define MPU6050_FF_THR 0x1D              // R/W
#define MPU6050_FF_DUR 0x1E              // R/W
#define MPU6050_MOT_THR 0x1F             // R/W
#define MPU6050_MOT_DUR 0x20             // R/W
#define MPU6050_ZRMOT_THR 0x21           // R/W
#define MPU6050_ZRMOT_DUR 0x22           // R/W
#define MPU6050_FIFO_EN 0x23             // R/W
#define MPU6050_I2C_MST_CTRL 0x24        // R/W
#define MPU6050_I2C_SLV0_ADDR 0x25       // R/W
#define MPU6050_I2C_SLV0_REG 0x26        // R/W
#define MPU6050_I2C_SLV0_CTRL 0x27       // R/W
#define MPU6050_I2C_SLV1_ADDR 0x28       // R/W
#define MPU6050_I2C_SLV1_REG 0x29        // R/W
#define MPU6050_I2C_SLV1_CTRL 0x2A       // R/W
#define MPU6050_I2C_SLV2_ADDR 0x2B       // R/W
#define MPU6050_I2C_SLV2_REG 0x2C        // R/W
#define MPU6050_I2C_SLV2_CTRL 0x2D       // R/W
#define MPU6050_I2C_SLV3_ADDR 0x2E       // R/W
#define MPU6050_I2C_SLV3_REG 0x2F        // R/W
#define MPU6050_I2C_SLV3_CTRL 0x30       // R/W
#define MPU6050_I2C_SLV4_ADDR 0x31       // R/W
#define MPU6050_I2C_SLV4_REG 0x32        // R/W
#define MPU6050_I2C_SLV4_DO 0x33         // R/W
#define MPU6050_I2C_SLV4_CTRL 0x34       // R/W
#define MPU6050_I2C_SLV4_DI 0x35         // R
#define MPU6050_I2C_MST_STATUS 0x36      // R
#define MPU6050_INT_PIN_CFG 0x37         // R/W
#define MPU6050_INT_ENABLE 0x38          // R/W
#define MPU6050_INT_STATUS 0x3A          // R
#define MPU6050_ACCEL_XOUT_H 0x3B        // R
#define MPU6050_ACCEL_XOUT_L 0x3C        // R
#define MPU6050_ACCEL_YOUT_H 0x3D        // R
#define MPU6050_ACCEL_YOUT_L 0x3E        // R
#define MPU6050_ACCEL_ZOUT_H 0x3F        // R
#define MPU6050_ACCEL_ZOUT_L 0x40        // R
#define MPU6050_TEMP_OUT_H 0x41          // R
#define MPU6050_TEMP_OUT_L 0x42          // R
#define MPU6050_GYRO_XOUT_H 0x43         // R
#define MPU6050_GYRO_XOUT_L 0x44         // R
#define MPU6050_GYRO_YOUT_H 0x45         // R
#define MPU6050_GYRO_YOUT_L 0x46         // R
#define MPU6050_GYRO_ZOUT_H 0x47         // R
#define MPU6050_GYRO_ZOUT_L 0x48         // R
#define MPU6050_EXT_SENS_DATA_00 0x49    // R
#define MPU6050_EXT_SENS_DATA_01 0x4A    // R
#define MPU6050_EXT_SENS_DATA_02 0x4B    // R
#define MPU6050_EXT_SENS_DATA_03 0x4C    // R
#define MPU6050_EXT_SENS_DATA_04 0x4D    // R
#define MPU6050_EXT_SENS_DATA_05 0x4E    // R
#define MPU6050_EXT_SENS_DATA_06 0x4F    // R
#define MPU6050_EXT_SENS_DATA_07 0x50    // R
#define MPU6050_EXT_SENS_DATA_08 0x51    // R
#define MPU6050_EXT_SENS_DATA_09 0x52    // R
#define MPU6050_EXT_SENS_DATA_10 0x53    // R
#define MPU6050_EXT_SENS_DATA_11 0x54    // R
#define MPU6050_EXT_SENS_DATA_12 0x55    // R
#define MPU6050_EXT_SENS_DATA_13 0x56    // R
#define MPU6050_EXT_SENS_DATA_14 0x57    // R
#define MPU6050_EXT_SENS_DATA_15 0x58    // R
#define MPU6050_EXT_SENS_DATA_16 0x59    // R
#define MPU6050_EXT_SENS_DATA_17 0x5A    // R
#define MPU6050_EXT_SENS_DATA_18 0x5B    // R
#define MPU6050_EXT_SENS_DATA_19 0x5C    // R
#define MPU6050_EXT_SENS_DATA_20 0x5D    // R
#define MPU6050_EXT_SENS_DATA_21 0x5E    // R
#define MPU6050_EXT_SENS_DATA_22 0x5F    // R
#define MPU6050_EXT_SENS_DATA_23 0x60    // R
#define MPU6050_MOT_DETECT_STATUS 0x61   // R
#define MPU6050_I2C_SLV0_DO 0x63         // R/W
#define MPU6050_I2C_SLV1_DO 0x64         // R/W
#define MPU6050_I2C_SLV2_DO 0x65         // R/W
#define MPU6050_I2C_SLV3_DO 0x66         // R/W
#define MPU6050_I2C_MST_DELAY_CTRL 0x67  // R/W
#define MPU6050_SIGNAL_PATH_RESET 0x68   // R/W
#define MPU6050_MOT_DETECT_CTRL 0x69     // R/W
#define MPU6050_USER_CTRL 0x6A           // R/W
#define MPU6050_PWR_MGMT_1 0x6B          // R/W
#define MPU6050_PWR_MGMT_2 0x6C          // R/W
#define MPU6050_FIFO_COUNTH 0x72         // R/W
#define MPU6050_FIFO_COUNTL 0x73         // R/W
#define MPU6050_FIFO_R_W 0x74            // R/W
#define MPU6050_WHO_AM_I 0x75            // R

// Defines for the bits, to be able to change
// between bit number and binary definition.
// By using the bit number, programming the sensor
// is like programming the AVR microcontroller.
// But instead of using "(1<<X)", or "_BV(X)",
// the Arduino "bit(X)" is used.
#define MPU6050_D0 0
#define MPU6050_D1 1
#define MPU6050_D2 2
#define MPU6050_D3 3
#define MPU6050_D4 4
#define MPU6050_D5 5
#define MPU6050_D6 6
#define MPU6050_D7 7

// AUX_VDDIO Register
#define MPU6050_AUX_VDDIO MPU6050_D7  // I2C high: 1=VDD, 0=VLOGIC

// CONFIG Register
// DLPF is Digital Low Pass Filter for both gyro and accelerometers.
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_DLPF_CFG0 MPU6050_D0
#define MPU6050_DLPF_CFG1 MPU6050_D1
#define MPU6050_DLPF_CFG2 MPU6050_D2
#define MPU6050_EXT_SYNC_SET0 MPU6050_D3
#define MPU6050_EXT_SYNC_SET1 MPU6050_D4
#define MPU6050_EXT_SYNC_SET2 MPU6050_D5

// Combined definitions for the EXT_SYNC_SET values
#define MPU6050_EXT_SYNC_SET_0 (0)
#define MPU6050_EXT_SYNC_SET_1 (bit(MPU6050_EXT_SYNC_SET0))
#define MPU6050_EXT_SYNC_SET_2 (bit(MPU6050_EXT_SYNC_SET1))
#define MPU6050_EXT_SYNC_SET_3 (bit(MPU6050_EXT_SYNC_SET1) | bit(MPU6050_EXT_SYNC_SET0))
#define MPU6050_EXT_SYNC_SET_4 (bit(MPU6050_EXT_SYNC_SET2))
#define MPU6050_EXT_SYNC_SET_5 (bit(MPU6050_EXT_SYNC_SET2) | bit(MPU6050_EXT_SYNC_SET0))
#define MPU6050_EXT_SYNC_SET_6 (bit(MPU6050_EXT_SYNC_SET2) | bit(MPU6050_EXT_SYNC_SET1))
#define MPU6050_EXT_SYNC_SET_7 (bit(MPU6050_EXT_SYNC_SET2) | bit(MPU6050_EXT_SYNC_SET1) | bit(MPU6050_EXT_SYNC_SET0))

// Alternative names for the combined definitions.
#define MPU6050_EXT_SYNC_DISABLED MPU6050_EXT_SYNC_SET_0
#define MPU6050_EXT_SYNC_TEMP_OUT_L MPU6050_EXT_SYNC_SET_1
#define MPU6050_EXT_SYNC_GYRO_XOUT_L MPU6050_EXT_SYNC_SET_2
#define MPU6050_EXT_SYNC_GYRO_YOUT_L MPU6050_EXT_SYNC_SET_3
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L MPU6050_EXT_SYNC_SET_4
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L MPU6050_EXT_SYNC_SET_5
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L MPU6050_EXT_SYNC_SET_6
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L MPU6050_EXT_SYNC_SET_7

// Combined definitions for the DLPF_CFG values
#define MPU6050_DLPF_CFG_0 (0)
#define MPU6050_DLPF_CFG_1 (bit(MPU6050_DLPF_CFG0))
#define MPU6050_DLPF_CFG_2 (bit(MPU6050_DLPF_CFG1))
#define MPU6050_DLPF_CFG_3 (bit(MPU6050_DLPF_CFG1) | bit(MPU6050_DLPF_CFG0))
#define MPU6050_DLPF_CFG_4 (bit(MPU6050_DLPF_CFG2))
#define MPU6050_DLPF_CFG_5 (bit(MPU6050_DLPF_CFG2) | bit(MPU6050_DLPF_CFG0))
#define MPU6050_DLPF_CFG_6 (bit(MPU6050_DLPF_CFG2) | bit(MPU6050_DLPF_CFG1))
#define MPU6050_DLPF_CFG_7 (bit(MPU6050_DLPF_CFG2) | bit(MPU6050_DLPF_CFG1) | bit(MPU6050_DLPF_CFG0))

// Alternative names for the combined definitions
// This name uses the bandwidth (Hz) for the accelometer,
// for the gyro the bandwidth is almost the same.
#define MPU6050_DLPF_260HZ MPU6050_DLPF_CFG_0
#define MPU6050_DLPF_184HZ MPU6050_DLPF_CFG_1
#define MPU6050_DLPF_94HZ MPU6050_DLPF_CFG_2
#define MPU6050_DLPF_44HZ MPU6050_DLPF_CFG_3
#define MPU6050_DLPF_21HZ MPU6050_DLPF_CFG_4
#define MPU6050_DLPF_10HZ MPU6050_DLPF_CFG_5
#define MPU6050_DLPF_5HZ MPU6050_DLPF_CFG_6
#define MPU6050_DLPF_RESERVED MPU6050_DLPF_CFG_7

// GYRO_CONFIG Register
// The XG_ST, YG_ST, ZG_ST are bits for selftest.
// The FS_SEL sets the range for the gyro.
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_FS_SEL0 MPU6050_D3
#define MPU6050_FS_SEL1 MPU6050_D4
#define MPU6050_ZG_ST MPU6050_D5
#define MPU6050_YG_ST MPU6050_D6
#define MPU6050_XG_ST MPU6050_D7

// Combined definitions for the FS_SEL values
#define MPU6050_FS_SEL_0 (0)
#define MPU6050_FS_SEL_1 (bit(MPU6050_FS_SEL0))
#define MPU6050_FS_SEL_2 (bit(MPU6050_FS_SEL1))
#define MPU6050_FS_SEL_3 (bit(MPU6050_FS_SEL1) | bit(MPU6050_FS_SEL0))

// Alternative names for the combined definitions
// The name uses the range in degrees per second.
#define MPU6050_FS_SEL_250 MPU6050_FS_SEL_0
#define MPU6050_FS_SEL_500 MPU6050_FS_SEL_1
#define MPU6050_FS_SEL_1000 MPU6050_FS_SEL_2
#define MPU6050_FS_SEL_2000 MPU6050_FS_SEL_3

// ACCEL_CONFIG Register
// The XA_ST, YA_ST, ZA_ST are bits for selftest.
// The AFS_SEL sets the range for the accelerometer.
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_ACCEL_HPF0 MPU6050_D0
#define MPU6050_ACCEL_HPF1 MPU6050_D1
#define MPU6050_ACCEL_HPF2 MPU6050_D2
#define MPU6050_AFS_SEL0 MPU6050_D3
#define MPU6050_AFS_SEL1 MPU6050_D4
#define MPU6050_ZA_ST MPU6050_D5
#define MPU6050_YA_ST MPU6050_D6
#define MPU6050_XA_ST MPU6050_D7

// Combined definitions for the ACCEL_HPF values
#define MPU6050_ACCEL_HPF_0 (0)
#define MPU6050_ACCEL_HPF_1 (bit(MPU6050_ACCEL_HPF0))
#define MPU6050_ACCEL_HPF_2 (bit(MPU6050_ACCEL_HPF1))
#define MPU6050_ACCEL_HPF_3 (bit(MPU6050_ACCEL_HPF1) | bit(MPU6050_ACCEL_HPF0))
#define MPU6050_ACCEL_HPF_4 (bit(MPU6050_ACCEL_HPF2))
#define MPU6050_ACCEL_HPF_7 (bit(MPU6050_ACCEL_HPF2) | bit(MPU6050_ACCEL_HPF1) | bit(MPU6050_ACCEL_HPF0))

// Alternative names for the combined definitions
// The name uses the Cut-off frequency.
#define MPU6050_ACCEL_HPF_RESET MPU6050_ACCEL_HPF_0
#define MPU6050_ACCEL_HPF_5HZ MPU6050_ACCEL_HPF_1
#define MPU6050_ACCEL_HPF_2_5HZ MPU6050_ACCEL_HPF_2
#define MPU6050_ACCEL_HPF_1_25HZ MPU6050_ACCEL_HPF_3
#define MPU6050_ACCEL_HPF_0_63HZ MPU6050_ACCEL_HPF_4
#define MPU6050_ACCEL_HPF_HOLD MPU6050_ACCEL_HPF_7

// Combined definitions for the AFS_SEL values
#define MPU6050_AFS_SEL_0 (0)
#define MPU6050_AFS_SEL_1 (bit(MPU6050_AFS_SEL0))
#define MPU6050_AFS_SEL_2 (bit(MPU6050_AFS_SEL1))
#define MPU6050_AFS_SEL_3 (bit(MPU6050_AFS_SEL1) | bit(MPU6050_AFS_SEL0))

// Alternative names for the combined definitions
// The name uses the full scale range for the accelerometer.
#define MPU6050_AFS_SEL_2G MPU6050_AFS_SEL_0
#define MPU6050_AFS_SEL_4G MPU6050_AFS_SEL_1
#define MPU6050_AFS_SEL_8G MPU6050_AFS_SEL_2
#define MPU6050_AFS_SEL_16G MPU6050_AFS_SEL_3

// FIFO_EN Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_SLV0_FIFO_EN MPU6050_D0
#define MPU6050_SLV1_FIFO_EN MPU6050_D1
#define MPU6050_SLV2_FIFO_EN MPU6050_D2
#define MPU6050_ACCEL_FIFO_EN MPU6050_D3
#define MPU6050_ZG_FIFO_EN MPU6050_D4
#define MPU6050_YG_FIFO_EN MPU6050_D5
#define MPU6050_XG_FIFO_EN MPU6050_D6
#define MPU6050_TEMP_FIFO_EN MPU6050_D7

// I2C_PIN_CFG Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_CLKOUT_EN MPU6050_D0
#define MPU6050_I2C_BYPASS_EN MPU6050_D1
#define MPU6050_FSYNC_INT_EN MPU6050_D2
#define MPU6050_FSYNC_INT_LEVEL MPU6050_D3
#define MPU6050_INT_RD_CLEAR MPU6050_D4
#define MPU6050_LATCH_INT_EN MPU6050_D5
#define MPU6050_INT_OPEN MPU6050_D6
#define MPU6050_INT_LEVEL MPU6050_D7

// INT_ENABLE Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_DATA_RDY_EN MPU6050_D0
#define MPU6050_I2C_MST_INT_EN MPU6050_D3
#define MPU6050_FIFO_OFLOW_EN MPU6050_D4
#define MPU6050_ZMOT_EN MPU6050_D5
#define MPU6050_MOT_EN MPU6050_D6
#define MPU6050_FF_EN MPU6050_D7

// INT_STATUS Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_DATA_RDY_INT MPU6050_D0
#define MPU6050_I2C_MST_INT MPU6050_D3
#define MPU6050_FIFO_OFLOW_INT MPU6050_D4
#define MPU6050_ZMOT_INT MPU6050_D5
#define MPU6050_MOT_INT MPU6050_D6
#define MPU6050_FF_INT MPU6050_D7

// MOT_DETECT_STATUS Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_MOT_ZRMOT MPU6050_D0
#define MPU6050_MOT_ZPOS MPU6050_D2
#define MPU6050_MOT_ZNEG MPU6050_D3
#define MPU6050_MOT_YPOS MPU6050_D4
#define MPU6050_MOT_YNEG MPU6050_D5
#define MPU6050_MOT_XPOS MPU6050_D6
#define MPU6050_MOT_XNEG MPU6050_D7

// IC2_MST_DELAY_CTRL Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_I2C_SLV0_DLY_EN MPU6050_D0
#define MPU6050_I2C_SLV1_DLY_EN MPU6050_D1
#define MPU6050_I2C_SLV2_DLY_EN MPU6050_D2
#define MPU6050_I2C_SLV3_DLY_EN MPU6050_D3
#define MPU6050_I2C_SLV4_DLY_EN MPU6050_D4
#define MPU6050_DELAY_ES_SHADOW MPU6050_D7

// SIGNAL_PATH_RESET Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_TEMP_RESET MPU6050_D0
#define MPU6050_ACCEL_RESET MPU6050_D1
#define MPU6050_GYRO_RESET MPU6050_D2

// MOT_DETECT_CTRL Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_MOT_COUNT0 MPU6050_D0
#define MPU6050_MOT_COUNT1 MPU6050_D1
#define MPU6050_FF_COUNT0 MPU6050_D2
#define MPU6050_FF_COUNT1 MPU6050_D3
#define MPU6050_ACCEL_ON_DELAY0 MPU6050_D4
#define MPU6050_ACCEL_ON_DELAY1 MPU6050_D5

// Combined definitions for the MOT_COUNT
#define MPU6050_MOT_COUNT_0 (0)
#define MPU6050_MOT_COUNT_1 (bit(MPU6050_MOT_COUNT0))
#define MPU6050_MOT_COUNT_2 (bit(MPU6050_MOT_COUNT1))
#define MPU6050_MOT_COUNT_3 (bit(MPU6050_MOT_COUNT1) | bit(MPU6050_MOT_COUNT0))

// Alternative names for the combined definitions
#define MPU6050_MOT_COUNT_RESET MPU6050_MOT_COUNT_0

// Combined definitions for the FF_COUNT
#define MPU6050_FF_COUNT_0 (0)
#define MPU6050_FF_COUNT_1 (bit(MPU6050_FF_COUNT0))
#define MPU6050_FF_COUNT_2 (bit(MPU6050_FF_COUNT1))
#define MPU6050_FF_COUNT_3 (bit(MPU6050_FF_COUNT1) | bit(MPU6050_FF_COUNT0))

// Alternative names for the combined definitions
#define MPU6050_FF_COUNT_RESET MPU6050_FF_COUNT_0

// Combined definitions for the ACCEL_ON_DELAY
#define MPU6050_ACCEL_ON_DELAY_0 (0)
#define MPU6050_ACCEL_ON_DELAY_1 (bit(MPU6050_ACCEL_ON_DELAY0))
#define MPU6050_ACCEL_ON_DELAY_2 (bit(MPU6050_ACCEL_ON_DELAY1))
#define MPU6050_ACCEL_ON_DELAY_3 (bit(MPU6050_ACCEL_ON_DELAY1) | bit(MPU6050_ACCEL_ON_DELAY0))

// Alternative names for the ACCEL_ON_DELAY
#define MPU6050_ACCEL_ON_DELAY_0MS MPU6050_ACCEL_ON_DELAY_0
#define MPU6050_ACCEL_ON_DELAY_1MS MPU6050_ACCEL_ON_DELAY_1
#define MPU6050_ACCEL_ON_DELAY_2MS MPU6050_ACCEL_ON_DELAY_2
#define MPU6050_ACCEL_ON_DELAY_3MS MPU6050_ACCEL_ON_DELAY_3

// USER_CTRL Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_SIG_COND_RESET MPU6050_D0
#define MPU6050_I2C_MST_RESET MPU6050_D1
#define MPU6050_FIFO_RESET MPU6050_D2
#define MPU6050_I2C_IF_DIS MPU6050_D4  // must be 0 for MPU-6050
#define MPU6050_I2C_MST_EN MPU6050_D5
#define MPU6050_FIFO_EN MPU6050_D6

// PWR_MGMT_1 Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_CLKSEL0 MPU6050_D0
#define MPU6050_CLKSEL1 MPU6050_D1
#define MPU6050_CLKSEL2 MPU6050_D2
#define MPU6050_TEMP_DIS MPU6050_D3      // 1: disable temperature sensor
#define MPU6050_CYCLE MPU6050_D5         // 1: sample and sleep
#define MPU6050_SLEEP MPU6050_D6         // 1: sleep mode
#define MPU6050_DEVICE_RESET MPU6050_D7  // 1: reset to default values

// Combined definitions for the CLKSEL
#define MPU6050_CLKSEL_0 (0)
#define MPU6050_CLKSEL_1 (bit(MPU6050_CLKSEL0))
#define MPU6050_CLKSEL_2 (bit(MPU6050_CLKSEL1))
#define MPU6050_CLKSEL_3 (bit(MPU6050_CLKSEL1) | bit(MPU6050_CLKSEL0))
#define MPU6050_CLKSEL_4 (bit(MPU6050_CLKSEL2))
#define MPU6050_CLKSEL_5 (bit(MPU6050_CLKSEL2) | bit(MPU6050_CLKSEL0))
#define MPU6050_CLKSEL_6 (bit(MPU6050_CLKSEL2) | bit(MPU6050_CLKSEL1))
#define MPU6050_CLKSEL_7 (bit(MPU6050_CLKSEL2) | bit(MPU6050_CLKSEL1) | bit(MPU6050_CLKSEL0))

// Alternative names for the combined definitions
#define MPU6050_CLKSEL_INTERNAL MPU6050_CLKSEL_0
#define MPU6050_CLKSEL_X MPU6050_CLKSEL_1
#define MPU6050_CLKSEL_Y MPU6050_CLKSEL_2
#define MPU6050_CLKSEL_Z MPU6050_CLKSEL_3
#define MPU6050_CLKSEL_EXT_32KHZ MPU6050_CLKSEL_4
#define MPU6050_CLKSEL_EXT_19_2MHZ MPU6050_CLKSEL_5
#define MPU6050_CLKSEL_RESERVED MPU6050_CLKSEL_6
#define MPU6050_CLKSEL_STOP MPU6050_CLKSEL_7

// PWR_MGMT_2 Register
// These are the names for the bits.
// Use these only with the bit() macro.
#define MPU6050_STBY_ZG MPU6050_D0
#define MPU6050_STBY_YG MPU6050_D1
#define MPU6050_STBY_XG MPU6050_D2
#define MPU6050_STBY_ZA MPU6050_D3
#define MPU6050_STBY_YA MPU6050_D4
#define MPU6050_STBY_XA MPU6050_D5
#define MPU6050_LP_WAKE_CTRL0 MPU6050_D6
#define MPU6050_LP_WAKE_CTRL1 MPU6050_D7

// Combined definitions for the LP_WAKE_CTRL
#define MPU6050_LP_WAKE_CTRL_0 (0)
#define MPU6050_LP_WAKE_CTRL_1 (bit(MPU6050_LP_WAKE_CTRL0))
#define MPU6050_LP_WAKE_CTRL_2 (bit(MPU6050_LP_WAKE_CTRL1))
#define MPU6050_LP_WAKE_CTRL_3 (bit(MPU6050_LP_WAKE_CTRL1) | bit(MPU6050_LP_WAKE_CTRL0))

// Alternative names for the combined definitions
// The names uses the Wake-up Frequency.
#define MPU6050_LP_WAKE_1_25HZ MPU6050_LP_WAKE_CTRL_0
#define MPU6050_LP_WAKE_2_5HZ MPU6050_LP_WAKE_CTRL_1
#define MPU6050_LP_WAKE_5HZ MPU6050_LP_WAKE_CTRL_2
#define MPU6050_LP_WAKE_10HZ MPU6050_LP_WAKE_CTRL_3

// Default I2C address for the MPU-6050 is 0x68.
#define MPU6050_I2C_ADDRESS 0x68


// Declaring an union for the registers and the axis values.
// The byte order does not match the byte order of
// the compiler and AVR chip.
// The AVR chip (on the Arduino board) has the Low Byte
// at the lower address.
// But the MPU-6050 has a different order: High Byte at
// lower address, so that has to be corrected.
// The register part "reg" is only used internally,
// and are swapped in code.
typedef union accel_t_gyro_union {
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct
  {
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;
    int16_t temperature;
    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
  } value;
};

void MPU6050_read_1axis();
void MPU6050_read_3axis();
void MPU6050_setup();
void MPU6050_calibrate();
float MPU6050_getAngle(float dt);
bool MPU6050_newData();
int MPU6050_read(int start, uint8_t *buffer, int size);
int MPU6050_write(int start, const uint8_t *pData, int size);
int MPU6050_write_reg(int reg, uint8_t data);

#endif /* MPU6050_H_ */
