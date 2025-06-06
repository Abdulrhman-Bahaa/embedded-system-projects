/**
 ******************************************************************************
 * \file       motors.hpp
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains declarations for the drone motors
 * \date       2025-05-16
 ******************************************************************************
*/

#ifndef UAV_MOTORS_HPP
#define UAV_MOTORS_HPP

/* Includes -----------------------------------------------------------------*/
#include <Arduino.h>
#include "application.hpp"

/* Macro Declarations -------------------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
class Motor {
  public:
    uint16_t pwm_value;

    /**
     * \brief Constructor for the Motor class
     * \param pin The pin number to which the motor is connected
     */
    Motor(uint8_t pin);

    /**
     * \brief Initializes the motor by setting the pin mode and initial PWM value
     */
    void init();

    /**
     * \brief Sets the PWM value for the motor
     * \param pwm_value The PWM value to set (0-255)
     */
    void set_pwm(uint16_t pwm_value);

  private:
    uint8_t pin;
};

#endif /* UAV_MOTORS_HPP*/