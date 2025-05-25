/**
 ******************************************************************************
 * \file       control.hpp
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains declarations for the drone control
 * \date       2025-05-16
 ******************************************************************************
*/

#ifndef UAV_CONTROL_HPP
#define UAV_CONTROL_HPP

/* Includes -----------------------------------------------------------------*/
#include <Arduino.h>

/* Macro Declarations -------------------------------------------------------*/
#define BACK_CALCULATION_ANTI_WINDUP  0

/* Data Type Declarations ---------------------------------------------------*/
class PidController {
    public:
      float kp, ki, kd, ka, sp;
      float min_roll_actuator_command, max_roll_actuator_command;
      float error, prev_error, accumulated_error;
      float raw_pid_output, actual_pid_output, proportional_term, integral_term, derivative_term;
      float dt, current_time, prev_time, saturated; 
  
      PidController(float kp, float ki, float kd, float ka, float sp,
                    float min_roll_actuator_command, float max_roll_actuator_command);
      void control(float pv);
};

#endif    /* UAV_CONTROL_HPP */