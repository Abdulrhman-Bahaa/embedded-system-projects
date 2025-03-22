/**
 ******************************************************************************
 * @file       application.h
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains declarations for the drone gcs
 * @date       2025-03-14
 ******************************************************************************
*/
#ifndef APPLICATION_H
#define	APPLICATION_H

/* Includes -----------------------------------------------------------------*/
#include "mcal_std_types.h"
#include "imu.h"
#include "gcs.h"

/* Macro Declarations -------------------------------------------------------*/
// Motors wiring
#define MOTOR_0_PIN 6
#define MOTOR_1_PIN 6
#define MOTOR_2_PIN 5
#define MOTOR_3_PIN 5

// PID controller initial configurations
#define ROLL_CONTROLLER_KP                    0
#define ROLL_CONTROLLER_KI                    0
#define ROLL_CONTROLLER_KD                    0
#define ROLL_CONTROLLER_KA                    0
#define ROLL_CONTROLLER_SP                    0
#define ROLL_CONTROLLER_MIN_ROLL_ACTUATOR_COMMAND  -127
#define ROLL_CONTROLLER_MAX_ROLL_ACTUATOR_COMMAND  127

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

#endif	/* APPLICATION_H */