/**
 ******************************************************************************
 * \file       application.hpp
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains declarations for the drone gcs
 * \date       2025-03-14
 ******************************************************************************
*/
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

/* Includes -----------------------------------------------------------------*/

/* Macro Declarations -------------------------------------------------------*/
#define MOTOR_0_PIN                                 18
#define MOTOR_1_PIN                                 18
#define MOTOR_2_PIN                                 19
#define MOTOR_3_PIN                                 19

#define MAX_MOTOR_PWM_VALUE                         255

// Pitch controller
#define PITCH_CONTROLLER_KP                         0
#define PITCH_CONTROLLER_KI                         0
#define PITCH_CONTROLLER_KD                         0
#define PITCH_CONTROLLER_KA                         0
#define PITCH_CONTROLLER_SP                         0
#define PITCH_CONTROLLER_MIN_PITCH_ACTUATOR_COMMAND -127
#define PITCH_CONTROLLER_MAX_PITCH_ACTUATOR_COMMAND 127

// Yaw controller
#define YAW_CONTROLLER_KP                           0
#define YAW_CONTROLLER_KI                           0
#define YAW_CONTROLLER_KD                           0
#define YAW_CONTROLLER_KA                           0
#define YAW_CONTROLLER_SP                           0
#define YAW_CONTROLLER_MIN_YAW_ACTUATOR_COMMAND     -127
#define YAW_CONTROLLER_MAX_YAW_ACTUATOR_COMMAND     127

// Roll controller
#define ROLL_CONTROLLER_KP                          0
#define ROLL_CONTROLLER_KI                          0
#define ROLL_CONTROLLER_KD                          0
#define ROLL_CONTROLLER_KA                          0
#define ROLL_CONTROLLER_SP                          0
#define ROLL_CONTROLLER_MIN_ROLL_ACTUATOR_COMMAND   -127
#define ROLL_CONTROLLER_MAX_ROLL_ACTUATOR_COMMAND   127

#endif /* APPLICATION_HPP */