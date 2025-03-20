/**
 ******************************************************************************
 * @file       gcs.h
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains declarations for the drone gcs
 * @date       2025-03-14
 ******************************************************************************
*/
#ifndef GCS_H
#define	GCS_H

/* Includes -----------------------------------------------------------------*/
#include <Arduino.h>

/* Macro Declarations -------------------------------------------------------*/
#define DATA_FROM_GCS_ARRAY_SIZE  6
#define DATA_OVER_SERIAL_IS_READABLE  1

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
class PidController {
    public:
      float kp, ki, kd, ka, sp;
      float min_roll_actuator_command, max_roll_actuator_command;
      float error, prev_error, accumulated_error;
      float pid_output, proportional_term, integral_term, derivative_term;
      float dt, current_time, prev_time; 
  
      PidController(float kp, float ki, float kd, float ka, float sp,
                    float min_roll_actuator_command, float max_roll_actuator_command);
      float compute(float pv);
      int8_t control();
};

/* Variables Declarations ---------------------------------------------------*/
// Spin motors(1)/Stop motors(0), Kp, Ki, Kd, Ka, SP, Min error, Max error
extern float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE];

/* Functions Declarations ---------------------------------------------------*/
void gcs_init();
void send_to_gcs(PidController *controller, float euler_angles[3], float motors_pwm[4]);
void receive_from_gcs(PidController *controller, float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE]);
void gcs_interface(PidController *controller, float euler_angles[3], float motors_pwm[4], float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE]);

#endif	/* GCS_H */
