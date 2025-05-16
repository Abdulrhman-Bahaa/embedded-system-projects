/**
 ******************************************************************************
 * @file       motors.cpp
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains implementations for the drone motors
 * @date       2025-05-16
 ******************************************************************************
*/
#include "motors.hpp"

/* Functions Implementations -------------------------------------------------*/

/**
 * @brief Constructor for the Motor class
 * @param pin The pin number to which the motor is connected
 */
Motor::Motor(uint8_t pin) {
    this->pin = pin;
    this->pwm_value = 0;
}

/**
 * @brief Initializes the motor by setting the pin mode and initial PWM value
 */
void Motor::init() {
    pinMode(pin, OUTPUT);
    analogWrite(pin, pwm_value);
}

/**
 * @brief Sets the PWM value for the motor
 * @param pwm_value The PWM value to set (0-255)
 */
void Motor::set_pwm(uint16_t pwm_value) {
    analogWrite(pin, (uint8_t)pwm_value);
}

