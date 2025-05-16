/**
 ******************************************************************************
 * @file       uav_gcs.cpp
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains implementations for the drone gcs
 * @date       2025-03-14
 ******************************************************************************
*/
#include "gcs.hpp"

/* Functions Implementations -------------------------------------------------*/
/**
 * @brief Constructor for the Gcs class
 * @param imu Pointer to the Imu object
 * @param motors Pointer to the Motor object
 * @param yaw_controller Pointer to the PidController object for yaw
 * @param pitch_controller Pointer to the PidController object for pitch
 * @param roll_controller Pointer to the PidController object for roll
 */
Gcs::Gcs(Imu* imu, Motor* motors, PidController* yaw_controller, PidController* pitch_controller, PidController* roll_controller) {
    this->imu = imu;
    this->motors = motors;
    this->yaw_controller = yaw_controller;
    this->pitch_controller = pitch_controller;
    this->roll_controller = roll_controller;
    this->motors_state = 0;
}

/**
 * @brief This class method initializes the GCS by setting up the serial communication.
 */
void Gcs::init() {
    Serial.begin(57600);
}

/**
 * @brief This class handles the communication with the ground control station (GCS).
 * @details The GCS class is responsible for sending and receiving data between the drone and the ground control station.
 *          Data shape: psi, theta, phi, motor_0_pwm, motor_1_pwm, motor_2_pwm, motor_3_pwm,
 *          yaw_controller_proportional_term, yaw_controller_integral_term, yaw_controller_derivative_term,
 *          pitch_controller_proportional_term, pitch_controller_integral_term, pitch_controller_derivative_term,
 *          roll_controller_proportional_term, roll_controller_integral_term, roll_controller_derivative_term,
 *          dummy_1, dummy_2, dummy_3
 * @note The data is sent in a comma-separated format.
 */
void Gcs::send() {
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
 * @brief This class method handles the reception of data from the ground control station (GCS).
 * @details The GCS class method is responsible for receiving data from the ground control station and updating the
 *          drone's state accordingly.
 *          Data shape: motors_state, yaw_controller_kp, yaw_controller_ki, yaw_controller_kd,    
 *          pitch_controller_kp, pitch_controller_ki, pitch_controller_kd,
 *          roll_controller_kp, roll_controller_ki, roll_controller_kd
 * @note The data is received in a comma-separated format.
 */
void Gcs::receive() {
    if (Serial.available() > 0) {
        String received_buffer = Serial.readStringUntil('\n');  //read received data

        char charArray[received_buffer.length() + 1];   // Create a char array
        received_buffer.toCharArray(charArray, sizeof(charArray)); // Convert String to char array

        char *token = strtok(charArray, ",");  // Get first token
        motors_state = atof(token);

        yaw_controller->kp = atof(strtok(NULL, ","));
        yaw_controller->ki = atof(strtok(NULL, ","));
        yaw_controller->kd = atof(strtok(NULL, ","));
        yaw_controller->ka = atof(strtok(NULL, ","));
        yaw_controller->sp = atof(strtok(NULL, ","));

        pitch_controller->kp = atof(strtok(NULL, ","));
        pitch_controller->ki = atof(strtok(NULL, ","));
        pitch_controller->kd = atof(strtok(NULL, ","));
        pitch_controller->ka = atof(strtok(NULL, ","));
        pitch_controller->sp = atof(strtok(NULL, ","));

        roll_controller->kp = atof(strtok(NULL, ","));
        roll_controller->ki = atof(strtok(NULL, ","));
        roll_controller->kd = atof(strtok(NULL, ","));
        roll_controller->ka = atof(strtok(NULL, ","));
        roll_controller->sp = atof(strtok(NULL, ","));
    }
}
 