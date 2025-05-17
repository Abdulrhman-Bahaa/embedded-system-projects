/**
 ******************************************************************************
 * \file       uav_gcs.hpp
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains declarations for the drone gcs
 * \date       2025-03-14
 ******************************************************************************
*/
#ifndef UAV_GCS_HPP
#define	UAV_GCS_HPP

/* Includes -----------------------------------------------------------------*/
#include "control.hpp"
#include "motors.hpp"
#include "imu.hpp"
#include <Arduino.h>

/* Macro Declarations -------------------------------------------------------*/
#define DATA_FROM_GCS_ARRAY_SIZE      13

/* Data type Declarations ---------------------------------------------------*/

/**
 * \brief This class is responsible for handling the communication with the Ground Control Station (GCS).
 * \details The Gcs class is responsible for sending and receiving data to and from the GCS.
 *          It uses serial communication to send and receive data.
 *          The data includes the euler angles, motor PWM values, and controller terms.
 */
class Gcs {
    private:
        Imu* imu;
        Motor* motors;
        PidController* yaw_controller;
        PidController* pitch_controller;
        PidController* roll_controller;
    public:
        uint8_t motors_state;
        /**
         * \brief Constructor for the Gcs class
         * \param imu Pointer to the Imu object
         * \param motors Pointer to the Motor object
         * \param yaw_controller Pointer to the PidController object for yaw
         * \param pitch_controller Pointer to the PidController object for pitch
         * \param roll_controller Pointer to the PidController object for roll
         */
        Gcs(Imu* imu, Motor* motors, PidController* yaw_controller, PidController* pitch_controller, PidController* roll_controller);

        /**
         * \brief Initializes the GCS by setting up the serial communication
         */
        void init();

        /**
         * \brief   This class method sends the data to the GCS.
         * \details The GCS class is responsible for sending data to the ground control station (GCS).
         *          The data includes the euler angles, motor PWM values, and controller terms.
         *          Data shape: monitor, euler_angles[0], euler_angles[1], euler_angles[2], motors[0].pwm_value, motors[1].pwm_value, motors[2].pwm_value, motors[3].pwm_value,
         *          yaw_controller->proportional_term, yaw_controller->integral_term, yaw_controller->derivative_term,
         *          pitch_controller->proportional_term, pitch_controller->integral_term, pitch_controller->derivative_term,
         *          roll_controller->proportional_term, roll_controller->integral_term, roll_controller->derivative_term
         * \note    The data is sent in a comma-separated forma without any spaces, ending with a newline character.
         */
        void send();

        /**
         * \brief   This class method receives data from the GCS and updates the controller parameters and motor state.
         * \details The GCS class is responsible for receiving data from the ground control station (GCS).
         *          The received data is parsed and used to update the controller parameters and motor state.
         *          Data shape: config, kp, ki, kd, ka, kp, ki, kd, ka, kp, ki, kd, ka
         *          or control, motors_state, yaw_setpoint, pitch_setpoint, roll_setpoint
         * \note    The data is received in a comma-separated format without any spaces, ending with a newline character.
         */
        void receive();
};

#endif	/* UAV_GCS_HPP */
