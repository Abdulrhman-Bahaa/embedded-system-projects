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

Gcs::Gcs(Imu* imu, Motor* motors, PidController* yaw_controller, PidController* pitch_controller,
         PidController* roll_controller)
    : imu(imu), motors(motors), yaw_controller(yaw_controller), pitch_controller(pitch_controller),
      roll_controller(roll_controller) {}

/**
 * \brief Initializes the GCS by setting up the serial communication
 * \details This method initializes the serial communication with the GCS at a baud rate of 57600.
 */
void
Gcs::init() {
    Serial.begin(115200);
}

void
Gcs::send() {
    if (Serial) {
        Serial.print("m,");
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
}

void
Gcs::receive() {
    if (Serial && Serial.available() > 0) {
        String received_buffer = Serial.readStringUntil('\n'); //read received data

        if (received_buffer.length() > 30) {
            Serial.println("error");
            return;
        }

        char charArray[received_buffer.length() + 1];              // Create a char array
        received_buffer.toCharArray(charArray, sizeof(charArray)); // Convert String to char array

        char* command_type = strtok(charArray, ","); // Get first token
        uint8_t controller = atof(strtok(NULL, ","));

        PidController* selected_controller = nullptr;

        switch (controller) {
            case 0: selected_controller = yaw_controller; break;
            case 1: selected_controller = pitch_controller; break;
            case 2: selected_controller = roll_controller; break;
            default: Serial.println("error"); return;
        }

        /* Control command (l)*/
        if (*command_type == 'l') {
            selected_controller->sp = atof(strtok(NULL, ","));
            /* Config command (g)*/
        } else if (*command_type == 'g') {
            selected_controller->kp = atof(strtok(NULL, ","));
            selected_controller->ki = atof(strtok(NULL, ","));
            selected_controller->kd = atof(strtok(NULL, ","));
            selected_controller->ka = atof(strtok(NULL, ","));
            Serial.println("ok");
        } else {
            Serial.println("error");
            return;
        }
    }
}
