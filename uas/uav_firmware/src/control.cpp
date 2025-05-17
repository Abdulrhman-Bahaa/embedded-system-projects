/**
 ******************************************************************************
 * \file       control.cpp
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains implementations for the drone control
 * \date       2025-05-16
 ******************************************************************************
*/
#include "control.hpp"

/* Functions Implementations -------------------------------------------------*/
PidController::PidController(float kp, float ki, float kd, float ka, float sp,
    float min_roll_actuator_command, float max_roll_actuator_command) {
    (this->kp) = kp;
    (this->ki) = ki;
    (this->kd) = kd;
    (this->ka) = ka;
    (this->sp) = sp;

    (this->min_roll_actuator_command) = min_roll_actuator_command;
    (this->max_roll_actuator_command) = max_roll_actuator_command;

    raw_pid_output = 0;
    actual_pid_output = 0;

    accumulated_error = 0;
    saturated = 0;
}

void PidController::control(float pv) {
    current_time = millis();
    dt = (current_time - prev_time) / 1000.0;  // Convert to seconds
    error = sp - pv;

    if (ki > 0) {
        #if (BACK_CALCULATION_ANTI_WINDUP == 1)
            accumulated_error += (error * dt);
        #else
            if ((!saturated) || ((actual_pid_output * error) < 0)) {
                accumulated_error += (error * dt);
            }
        #endif
    }
    else {
        accumulated_error = 0;
    }

    proportional_term = (kp * error);
    derivative_term = (kd * ((error - prev_error) / dt));
    integral_term = (ki * accumulated_error);

    raw_pid_output =  proportional_term + integral_term  + derivative_term;
    actual_pid_output =  constrain(raw_pid_output, min_roll_actuator_command, max_roll_actuator_command);


    #if (BACK_CALCULATION_ANTI_WINDUP == 1)
        accumulated_error -= (ka * (raw_pid_output - actual_pid_output));
    #else
        if ((actual_pid_output == max_roll_actuator_command) || (actual_pid_output == min_roll_actuator_command)) {
            saturated = 1;
        }
        else {
            saturated = 0;
        }
    #endif

    // Time update and error 
    prev_time = current_time;
    prev_error = error;
    current_time = millis();
}