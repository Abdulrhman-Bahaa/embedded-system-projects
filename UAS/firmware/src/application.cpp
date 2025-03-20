#include "application.h"

PidController rollController(ROLL_CONTROLLER_KP, ROLL_CONTROLLER_KI, ROLL_CONTROLLER_KD, ROLL_CONTROLLER_KA,
  ROLL_CONTROLLER_SP, ROLL_CONTROLLER_MIN_ROLL_ACTUATOR_COMMAND, 
  ROLL_CONTROLLER_MAX_ROLL_ACTUATOR_COMMAND);;

float euler_angles[3] = {1, 2, 3};
float motors_pwm[4];
float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE] = {0};


float roll_actuator_command = 0, altitude_actuator_command = 128;
MPU6050 mpu6050(Wire);


void setup() {
  gcs_init();
  imu_init();

  pinMode(MOTOR_0_PIN, OUTPUT);
  pinMode(MOTOR_2_PIN, OUTPUT);
  pinMode(13, OUTPUT);
}
void loop() {   
  gcs_interface(&rollController, euler_angles, motors_pwm, data_from_gcs);

  imu_get_angles(euler_angles);
  
  // Get the mapped output from the pid
  rollController.compute(euler_angles[2]);

  roll_actuator_command = rollController.control();

  motors_pwm[0] = altitude_actuator_command + roll_actuator_command;
  motors_pwm[1] = altitude_actuator_command + roll_actuator_command;
  motors_pwm[2] = altitude_actuator_command - roll_actuator_command;
  motors_pwm[3] = altitude_actuator_command - roll_actuator_command;

  // Apply action on the plant
  if (data_from_gcs[0] == 1) {
    analogWrite(MOTOR_0_PIN, motors_pwm[0]);
    analogWrite(MOTOR_2_PIN, motors_pwm[2]);
    digitalWrite(13, HIGH);
  }
  else {
    analogWrite(MOTOR_0_PIN, 0);
    analogWrite(MOTOR_2_PIN, 0);
    digitalWrite(13, LOW);
  }
}

