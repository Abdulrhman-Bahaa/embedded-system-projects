/**
 ******************************************************************************
 * @mainpage   UAV Project
 * @brief      Quadcopter Control System With Simple Ground Control Station
 *
 * ASCII representation of the quadcopter:
 *            (Front)
 *           4   ^   1
 *            \  |  /
 *             \ | /
 *      +------------------+   
 *             / | \
 *            /  |  \
 *           3   v   2
 *
 * - Propeller Layout:
 *   - **1 & 4** → Front propellers.
 *   - **2 & 3** → Rear propellers.
 *   - ϕ > 0 → Right tilt (clockwise roll).
 *   - ϕ < 0 → Left tilt (counterclockwise roll). *
 * @date       2025-03-21
 ******************************************************************************
 */
#include "application.h"

/* Variables Definitions ----------------------------------------------------*/ 
// Create roll controller using PID 
PidController rollController(ROLL_CONTROLLER_KP, ROLL_CONTROLLER_KI, ROLL_CONTROLLER_KD, ROLL_CONTROLLER_KA,
  ROLL_CONTROLLER_SP, ROLL_CONTROLLER_MIN_ROLL_ACTUATOR_COMMAND, 
  ROLL_CONTROLLER_MAX_ROLL_ACTUATOR_COMMAND);

// Data to be send to gcs (For monitoring)
float euler_angles[3] = {0};
uint8_t motors_pwm[4] = {0};
// Data received from gcs (For commands like tilting)
float data_from_gcs[DATA_FROM_GCS_ARRAY_SIZE] = {0};
// roll_actuator_command value will be changed by the roll controller
// altitude_actuator_command value will be fixed (Since no altitude controller is implemented)
float roll_actuator_command = 0, altitude_actuator_command = 128;

/* Main Function ------------------------------------------------------------*/
int main(void) {
  Std_ReturnType ret = E_OK;
  ret = application_initialize();
  while(1) {
    // The function which is responsible for sending and receiveing data from the gcs
    gcs_interface(&rollController, euler_angles, motors_pwm, data_from_gcs);

    // Get the euler angles
    imu_get_angles(euler_angles);
    
    // Get the control command from the controller
    rollController.control(euler_angles[2]);
    roll_actuator_command = rollController.actual_pid_output;

    motors_pwm[0] = altitude_actuator_command - roll_actuator_command;
    motors_pwm[1] = altitude_actuator_command - roll_actuator_command;
    motors_pwm[2] = altitude_actuator_command + roll_actuator_command;
    motors_pwm[3] = altitude_actuator_command + roll_actuator_command;

    // Apply action on the plant when data_from_gcs[0] is one (Changed from gcs)
    if (data_from_gcs[0] == 1) {
      analogWrite(MOTOR_0_PIN, motors_pwm[0]);
      analogWrite(MOTOR_2_PIN, motors_pwm[2]);
    }
    else {
      analogWrite(MOTOR_0_PIN, 0);
      analogWrite(MOTOR_2_PIN, 0);
    }
  }
  return 0;
}

/* Functions Implementations -------------------------------------------------*/
/**
 * @brief       This function will include calls of functions to initialize 
 *              application's interfaces
 * @return      Returns E_OK(0x01) or E_NOT_OK(0x00) (succeeded and not succeeded)
*/
Std_ReturnType application_initialize(void) {
  Std_ReturnType ret = E_OK;
  // Initialize some core peripherals like timers
  init();

  // Initialize serial communication (USART/57600 baudrate) for GCS
  gcs_init();

  // Initialize IMU (MPU6050 is used without DMP)
  imu_init();

  // Initialize PWM for propellers (Each two motors has the same PWM control, motor0 with motor1 and motor2 with motor3)
  pinMode(MOTOR_0_PIN, OUTPUT);
  pinMode(MOTOR_2_PIN, OUTPUT);
  
  return ret;
}
