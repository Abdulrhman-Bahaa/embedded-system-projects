/**
 ******************************************************************************
 * @file       gcs.cpp
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains implementations for the drone gcs
 * @date       2025-03-14
 ******************************************************************************
*/
#include "gcs.h"

/* Variables Definitions ----------------------------------------------------*/

/* Functions Implementations -------------------------------------------------*/
void gcs_init() {
    Serial.begin(57600);
}

PidController::PidController(float kp, float ki, float kd, float ka, float sp,
    float min_roll_actuator_command, float max_roll_actuator_command) {
    (this->kp) = kp;
    (this->ki) = ki;
    (this->kd) = kd;
    (this->ka) = ka;
    (this->sp) = sp;

    (this->min_roll_actuator_command) = min_roll_actuator_command;
    (this->max_roll_actuator_command) = max_roll_actuator_command;

    accumulated_error = 0;
}

float PidController::compute(float pv) {
    current_time = millis();
    dt = (current_time - prev_time) / 1000.0;  // Convert to seconds
    accumulated_error += (error * dt);
    error = sp - pv;

    proportional_term = kp * error;
    integral_term = ki * accumulated_error;
    derivative_term = kd * ((error - prev_error) / dt);

    pid_output = proportional_term + integral_term + derivative_term;

    // Time update and error
    prev_time = current_time;
    prev_error = error;
    current_time = millis();

    return pid_output;
}

int8_t PidController::control() {
    // Anti-windup
    integral_term -= (ka * (pid_output - constrain(pid_output, min_roll_actuator_command, max_roll_actuator_command)));
    pid_output = constrain((proportional_term + integral_term + derivative_term), min_roll_actuator_command, max_roll_actuator_command);

    return pid_output;
}

void send_to_gcs(PidController *controller, float euler_angles[3], float motors_pwm[4]) {
    // Send drone state variables, pid output and pwm signal for visualization

    #if (DATA_OVER_SERIAL_IS_READABLE == 1)
        for(uint8_t i = 0; i < 3; i++) {
            Serial.print(euler_angles[i]);
            Serial.print(",");
        }

        Serial.print(controller->proportional_term);
        Serial.print(",");
        Serial.print(controller->integral_term);
        Serial.print(",");
        Serial.print(controller->derivative_term);
        Serial.print(",");

        for(uint8_t i = 0; i < 4; i++) {
            if (i < 4 - 1) {
                Serial.print(motors_pwm[i]);
                Serial.print(",");
            }
            else {
                Serial.println(motors_pwm[i]);
            }
        }

    #else
        byte buffer[12];  // Create a byte buffer

        memcpy(buffer, euler_angles, 12);  // Copy float data to byte buffer
        Serial.write(255);  // Send start marker (sync byte)

        Serial.write(buffer, 12);  // Send float data as bytes

        // memcpy(buffer, &(controller->proportional_term), 4);
        // Serial.write(buffer, 4);  // Send float data as bytes

        // memcpy(buffer, &(controller->integral_term), 4);
        // Serial.write(buffer, 4);  // Send float data as bytes

        // memcpy(buffer, &(controller->derivative_term), 4);
        // Serial.write(buffer, 4);  // Send float data as bytes

        // memcpy(buffer, motors_pwm, 16);  // Copy float data to byte buffer
        // Serial.write(buffer, 16);  // Send float data as bytes


    #endif
}
  
void receive_from_gcs(PidController *controller, float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE]) {
    if(Serial.available() > 0) {   
        String received_buffer = Serial.readStringUntil('\n');  //read received data  
  
        char charArray[received_buffer.length() + 1];   // Create a char array
        received_buffer.toCharArray(charArray, sizeof(charArray)); // Convert String to char array
    
        char *token = strtok(charArray, ",");  // Get first token
        data_from_gcs[0] = atof(token);
  
        for (uint8_t i = 1; i < DATA_FROM_GCS_ARRAY_SIZE; i++) {
            data_from_gcs[i] = atof(strtok(NULL, ","));
        }

        controller->kp = data_from_gcs[1];
        controller->ki = data_from_gcs[2];
        controller->kd = data_from_gcs[3];
        controller->ka = data_from_gcs[4];
        controller->sp = data_from_gcs[5];
    }
}

void gcs_interface(PidController *controller, float euler_angles[3], float motors_pwm[4], float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE]) {
    send_to_gcs(controller, euler_angles, motors_pwm);
    receive_from_gcs(controller, data_from_gcs);
}

