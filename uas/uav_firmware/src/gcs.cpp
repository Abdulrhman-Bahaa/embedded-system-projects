/**
 ******************************************************************************
 * @file       uav_gcs.cpp
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

void send_to_gcs(PidController *controller, float euler_angles[3], uint8_t motors_pwm[4]) {
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
            Serial.print(motors_pwm[i]);
            Serial.print(",");
        }

        // Replace zeros for debug
        Serial.print(0);
        Serial.print(",");
        Serial.print(0);
        Serial.print(",");
        Serial.println(0);

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

void gcs_interface(PidController *controller, float euler_angles[3], uint8_t motors_pwm[4], float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE]) {
    send_to_gcs(controller, euler_angles, motors_pwm);
    receive_from_gcs(controller, data_from_gcs);
}

