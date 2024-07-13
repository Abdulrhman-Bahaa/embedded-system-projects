 /**
 ******************************************************************************
 * @file       ecual_ultrasonic_sensors.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for ultrasonic_sensors
 * @date       2024-2-13
 ******************************************************************************
*/
#ifndef ECUAL_ULTRASONIC_SENSORS_H
#define	ECUAL_ULTRASONIC_SENSORS_H

/* Includes -----------------------------------------------------------------*/
#include <gpio/mcal_gpio.h>
#include <timers/mcal_timer0.h>
#include <interrupts/external/mcal_external_interrupts.h>

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
typedef struct {
    pin_config_t trigger_pin;
    pin_change_interrupt_pins_t echo_pin;
}us_sensor_config_t;

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType ecual_us_sensor_init(const us_sensor_config_t* us_sensor);
Std_ReturnType ecual_us_sensor_trig(const us_sensor_config_t* us_sensor);
void us_sensor_app_interrupt_callback(void);
void timer0_app_interrupt_callback(void);
Std_ReturnType ecual_us_sensor_calc_distant(const us_sensor_config_t* us_sensor, uint16_t* distance);
Std_ReturnType ecual_us_sensor_invoke_on_range(const us_sensor_config_t* us_sensor, const uint16_t range_start, const uint16_t range_end, 
                                               void (*true_function_to_invoke)(void), void (*false_function_to_invoke)(void));
#endif	/* ECUAL_ULTRASONIC_SENSORS_H */
