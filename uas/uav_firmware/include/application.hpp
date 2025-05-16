/**
 ******************************************************************************
 * @file       application.hpp
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains declarations for the drone gcs
 * @date       2025-03-14
 ******************************************************************************
*/
#ifndef APPLICATION_HPP
#define	APPLICATION_HPP

/* Includes -----------------------------------------------------------------*/
#include "std_types.hpp"
#include "imu.hpp"
#include "gcs.hpp"
#include "control.hpp"
#include "motors.hpp"

/* Macro Declarations -------------------------------------------------------*/
#define MOTOR_0_PIN 3
#define MOTOR_1_PIN 5
#define MOTOR_2_PIN 6
#define MOTOR_3_PIN 9

// Pitch controller
#define PITCH_CONTROLLER_KP                          1
#define PITCH_CONTROLLER_KI                          1
#define PITCH_CONTROLLER_KD                          1
#define PITCH_CONTROLLER_KA                          0
#define PITCH_CONTROLLER_SP                          10
#define PITCH_CONTROLLER_MIN_PITCH_ACTUATOR_COMMAND  -127
#define PITCH_CONTROLLER_MAX_PITCH_ACTUATOR_COMMAND  127

// Yaw controller
#define YAW_CONTROLLER_KP                            1
#define YAW_CONTROLLER_KI                            1
#define YAW_CONTROLLER_KD                            1
#define YAW_CONTROLLER_KA                            0
#define YAW_CONTROLLER_SP                            10
#define YAW_CONTROLLER_MIN_YAW_ACTUATOR_COMMAND      -127
#define YAW_CONTROLLER_MAX_YAW_ACTUATOR_COMMAND      127

// Roll controller
#define ROLL_CONTROLLER_KP                           1
#define ROLL_CONTROLLER_KI                           1
#define ROLL_CONTROLLER_KD                           1
#define ROLL_CONTROLLER_KA                           0
#define ROLL_CONTROLLER_SP                           10
#define ROLL_CONTROLLER_MIN_ROLL_ACTUATOR_COMMAND    -127
#define ROLL_CONTROLLER_MAX_ROLL_ACTUATOR_COMMAND    127

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
/**
 * @brief       This function will include calls of functions to initialize 
 *              application's interfaces
 * @return      Returns E_OK(0x01) or E_NOT_OK(0x00) (succeeded and not succeeded)
*/
Std_ReturnType application_initialize(void);

#endif	/* APPLICATION_HPP */