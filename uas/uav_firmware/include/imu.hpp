/**
 ******************************************************************************
 * @file       imu.hpp
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains declarations for the imu functions
 * @date       2025-03-17
 ******************************************************************************
*/
#ifndef IMU_HPP
#define	IMU_HPP

/* Macro Declarations -------------------------------------------------------*/
#define DMP 0

/* Includes -----------------------------------------------------------------*/
#if (DMP == 1)
  #include "I2Cdev.h"
  #include "MPU6050_6Axis_MotionApps20.h"
#else
  #include <MPU6050_tockn.h>
#endif

/* Data Type Declarations ---------------------------------------------------*/
class Imu {
    MPU6050* mpu;
  public:
    float euler_angles[3];

    Imu();

    void init();
    void update();
};

#endif	/* IMU_HPP */
