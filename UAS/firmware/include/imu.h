/**
 ******************************************************************************
 * @file       imu.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains declarations for the imu functions
 * @date       2025-03-17
 ******************************************************************************
*/
#ifndef IMU_H
#define	IMU_H

/* Macro Declarations -------------------------------------------------------*/
#define DMP 0

/* Includes -----------------------------------------------------------------*/
#if (DMP == 1)
  #include "I2Cdev.h"
  #include "MPU6050_6Axis_MotionApps20.h"
#else
  #include <MPU6050_tockn.h>
#endif

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
void mpu6050_with_dmp_init();
void mpu6050_without_dmp_init();
void mpu6050_with_dmp_get_angles(float euler_angles[3]);
void mpu6050_without_dmp_get_angles(float euler_angles[3]);
void imu_init();
void imu_get_angles(float euler_angles[3]);

#endif	/* IMU_H */
