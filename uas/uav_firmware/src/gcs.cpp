/**
 ******************************************************************************
 * \file       uav_gcs.cpp
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains implementations for the drone gcs
 * \date       2025-03-14
 ******************************************************************************
*/
#include "gcs.hpp"

/* Functions Implementations -------------------------------------------------*/
/**
 * \brief                  Constructor for the Gcs class
 * \param imu              Pointer to the Imu object
 * \param motors           Pointer to the Motor object
 * \param yaw_controller   Pointer to the PidController object for yaw
 * \param pitch_controller Pointer to the PidController object for pitch
 * \param roll_controller  Pointer to the PidController object for roll
 */
Gcs::Gcs(Imu* imu, Motor* motors, PidController* yaw_controller, PidController* pitch_controller,
         PidController* roll_controller)
    : imu(imu), motors(motors), yaw_controller(yaw_controller), pitch_controller(pitch_controller),
      roll_controller(roll_controller), motors_state(0) {}

/**
 * \brief This class method initializes the GCS by setting up the serial communication.
 */
void
Gcs::init() {
    Serial.begin(57600);
}

/**
 * \brief   This class method sends the data to the GCS.
 * \details The GCS class is responsible for sending data to the ground control station (GCS).
 *          The data includes the euler angles, motor PWM values, and controller terms.
 *          Data shape: monitor, euler_angles[0], euler_angles[1], euler_angles[2], motors[0].pwm_value, motors[1].pwm_value, motors[2].pwm_value, motors[3].pwm_value,
 *          yaw_controller->proportional_term, yaw_controller->integral_term, yaw_controller->derivative_term,
 *          pitch_controller->proportional_term, pitch_controller->integral_term, pitch_controller->derivative_term,
 *          roll_controller->proportional_term, roll_controller->integral_term, roll_controller->derivative_term,
 *          debug0, debug1, debug2
 * \note    The data is sent in a comma-separated forma without any spaces, ending with a newline character.
 */
void
Gcs::send() {
    Serial.print("monitor,");
    for (uint8_t i = 0; i < 3; i++) {
        Serial.print(imu->euler_angles[i]);
        Serial.print(",");
    }
    for (uint8_t i = 0; i < 4; i++) {
        Serial.print(motors[i].pwm_value);
        Serial.print(",");
    }
    Serial.print(yaw_controller->proportional_term);
    Serial.print(",");
    Serial.print(yaw_controller->integral_term);
    Serial.print(",");
    Serial.print(yaw_controller->derivative_term);
    Serial.print(",");
    Serial.print(pitch_controller->proportional_term);
    Serial.print(",");
    Serial.print(pitch_controller->integral_term);
    Serial.print(",");
    Serial.print(pitch_controller->derivative_term);
    Serial.print(",");
    Serial.print(roll_controller->proportional_term);
    Serial.print(",");
    Serial.print(roll_controller->integral_term);
    Serial.print(",");
    Serial.print(roll_controller->derivative_term);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.println(0);
}

/**
 * \brief   This class method receives data from the GCS and updates the controller parameters and motor state.
 * \details The GCS class is responsible for receiving data from the ground control station (GCS).
 *          The received data is parsed and used to update the controller parameters and motor state.
 *          Data shape: config, kp, ki, kd, ka, kp, ki, kd, ka, kp, ki, kd, ka
 *          or control, motors_state, yaw_setpoint, pitch_setpoint, roll_setpoint
 * \note    The data is received in a comma-separated format without any spaces, ending with a newline character.
 */
void
Gcs::receive() {
    if (Serial.available() > 0) {
        String received_buffer = Serial.readStringUntil('\n'); //read received data

        char charArray[received_buffer.length() + 1];              // Create a char array
        received_buffer.toCharArray(charArray, sizeof(charArray)); // Convert String to char array

        char* token = strtok(charArray, ","); // Get first token

        if (String("config") == token) {
            Serial.println(token);

            /* Size Check */
            if (sizeof(charArray) / sizeof(charArray[0]) != 13) {
                Serial.println("Invalid data size");
                return;
            } else {
                yaw_controller->kp = atof(strtok(NULL, ","));
                yaw_controller->ki = atof(strtok(NULL, ","));
                yaw_controller->kd = atof(strtok(NULL, ","));
                yaw_controller->ka = atof(strtok(NULL, ","));
                pitch_controller->kp = atof(strtok(NULL, ","));
                pitch_controller->ki = atof(strtok(NULL, ","));
                pitch_controller->kd = atof(strtok(NULL, ","));
                pitch_controller->ka = atof(strtok(NULL, ","));
                roll_controller->kp = atof(strtok(NULL, ","));
                roll_controller->ki = atof(strtok(NULL, ","));
                roll_controller->kd = atof(strtok(NULL, ","));
                roll_controller->ka = atof(strtok(NULL, ","));
            }
        } else if (String("control") == token) {
            Serial.println(token);

            /* Size Check */
            if (sizeof(charArray) / sizeof(charArray[0]) != 4) {
                Serial.println("Invalid data size");
                return;
            } else {
                motors_state = atoi(strtok(NULL, ","));
                yaw_controller->sp = atof(strtok(NULL, ","));
                pitch_controller->sp = atof(strtok(NULL, ","));
                roll_controller->sp = atof(strtok(NULL, ","));
            }
        } else {
            Serial.println("Invalid command");
            return;
        }
    }
}
