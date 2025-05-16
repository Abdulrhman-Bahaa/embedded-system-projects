/**
 ******************************************************************************
 * @file       uav_gcs.hpp
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains declarations for the drone gcs
 * @date       2025-03-14
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
class Gcs {
    private:
        Imu* imu;
        Motor* motors;
        PidController* yaw_controller;
        PidController* pitch_controller;
        PidController* roll_controller;
    public:
        uint8_t motors_state;
        Gcs(Imu* imu, Motor* motors, PidController* yaw_controller, PidController* pitch_controller, PidController* roll_controller);
        void init();
        void send();
        void receive();
};

#endif	/* UAV_GCS_HPP */
