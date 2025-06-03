/**
 ******************************************************************************
 * \mainpage   UAV Project
 * \brief      Quadcopter Control System With Simple Ground Control Station
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
 * - Euler Angles in NED Frame (Right-Handed System)
 *   | Angle              | Axis      | Positive Rotation              | Negative Rotation                    |
 *   | ------------------ | --------- | ------------------------------ | ------------------------------------ |
 *   | Roll (φ -> phi)    | X (North) | Right wing down (right roll)   | Left wing down (left roll)           |
 *   | Pitch (θ -> theta) | Y (East)  | Nose up (climb)                | Nose down (dive)                     |
 *   | Yaw (ψ -> psi)     | Z (Down)  | Clockwise turn (to the right)  | Counter-clockwise turn (to the left) |
 * 
 * \date    2025-03-21
 * \author  Abdulrhman Bahaa
 * \see     https://github.com/Abdulrhman-Bahaa/embedded-system-projects/tree/main/uas
 ******************************************************************************
 */

/* includes ----------------------------------------------------------------*/

#include "application.hpp"
#include <USB/PluggableUSBSerial.h>
#include "control.hpp"
#include "gcs.hpp"
#include "imu.hpp"
#include "motors.hpp"
#include "std_types.hpp"

/* Functions Declarations ---------------------------------------------------*/

/**
 * \brief       This function will include calls of functions to initialize 
 *              application's interfaces
 * \return      Returns E_OK(0x01) or E_NOT_OK(0x00) (succeeded and not succeeded)
*/
Std_ReturnType application_initialize(void);

/* Variables Definitions ----------------------------------------------------*/

/* Create Yaw controller using PID */
PidController yaw_controller(YAW_CONTROLLER_KP, YAW_CONTROLLER_KI, YAW_CONTROLLER_KD, YAW_CONTROLLER_KA,
                             YAW_CONTROLLER_SP, YAW_CONTROLLER_MIN_YAW_ACTUATOR_COMMAND,
                             YAW_CONTROLLER_MAX_YAW_ACTUATOR_COMMAND);

/* Create Pitch controller using PID */
PidController pitch_controller(PITCH_CONTROLLER_KP, PITCH_CONTROLLER_KI, PITCH_CONTROLLER_KD, PITCH_CONTROLLER_KA,
                               PITCH_CONTROLLER_SP, PITCH_CONTROLLER_MIN_PITCH_ACTUATOR_COMMAND,
                               PITCH_CONTROLLER_MAX_PITCH_ACTUATOR_COMMAND);

/* Create Roll controller using PID */
PidController roll_controller(ROLL_CONTROLLER_KP, ROLL_CONTROLLER_KI, ROLL_CONTROLLER_KD, ROLL_CONTROLLER_KA,
                              ROLL_CONTROLLER_SP, ROLL_CONTROLLER_MIN_ROLL_ACTUATOR_COMMAND,
                              ROLL_CONTROLLER_MAX_ROLL_ACTUATOR_COMMAND);

/* Create Motors objects */
Motor motors[4] = {Motor(MOTOR_0_PIN), Motor(MOTOR_1_PIN), Motor(MOTOR_2_PIN), Motor(MOTOR_3_PIN)};

/* Create Imu object */
Imu imu;

/* Create GCS object */
Gcs gcs(&imu, motors, &yaw_controller, &pitch_controller, &roll_controller);

/* altitude_actuator_command value will be fixed (Since no altitude controller is implemented) */
uint16_t yaw_actuator_command, pitch_actuator_command, roll_actuator_command, altitude_actuator_command = 128;

/* Main Function ------------------------------------------------------------*/

int
main(void) {
    Std_ReturnType ret = E_OK;
    ret |= application_initialize();

    if (E_OK != ret) {
        /* Error in initialization */
        digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED to indicate error
        while (1)
            ;
    }

    /* Main loop */
    while (1) {

        /* Get the euler angles */
        imu.update();

        /* Get the data from the GCS */
        gcs.receive();

        /* Send the data to the GCS */
        gcs.send();

        /* Get the control command from the controllers */
        yaw_controller.control(imu.euler_angles[0]);
        pitch_controller.control(imu.euler_angles[1]);
        roll_controller.control(imu.euler_angles[2]);

        yaw_actuator_command = yaw_controller.actual_pid_output;
        pitch_actuator_command = pitch_controller.actual_pid_output;
        roll_actuator_command = roll_controller.actual_pid_output;

        /* Apply action on the plant*/
        /* Motor mixing formula */
        motors[0].set_pwm(altitude_actuator_command - pitch_actuator_command - yaw_actuator_command
                          - roll_actuator_command);
        motors[1].set_pwm(altitude_actuator_command + pitch_actuator_command - yaw_actuator_command
                          - roll_actuator_command);
        motors[2].set_pwm(altitude_actuator_command + pitch_actuator_command + yaw_actuator_command
                          + roll_actuator_command);
        motors[3].set_pwm(altitude_actuator_command - pitch_actuator_command + yaw_actuator_command
                          + roll_actuator_command);
    }
    return 0;
}

/* Functions Implementations -------------------------------------------------*/

Std_ReturnType
application_initialize(void) {
    Std_ReturnType ret = E_OK;
    /* Initialize some core peripherals like timers */
    init();

    /* Initialize the USB device */
    PluggableUSBD().begin();
    _SerialUSB.begin(115200);

    /* Initialize GCS */
    gcs.init();

    /* Initialize IMU (MPU6050 is used without DMP) */
    imu.init();

    /* Initialize motors */
    motors[0].init();
    motors[1].init();
    motors[2].init();
    motors[3].init();

    return ret;
}
