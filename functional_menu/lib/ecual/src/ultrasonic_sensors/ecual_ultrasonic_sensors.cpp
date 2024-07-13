// /**
//  ******************************************************************************
//  * @file       ecual_ultrasonic_sensors.c
//  * @author     Abdulrhman Bahaa
//  * @brief      This source file contains the implementations for ultrasonic_sensors
//  * @date       2024-2-13
//  ******************************************************************************
// */
// #include "ecual_ultrasonic_sensors.h"

// /* Variables Definitions -----------------------------------------------------*/
// volatile uint16_t timer0_reg_value = 0;
// volatile uint8_t num_of_overflows = 0;


// /* Functions Implementations -------------------------------------------------*/
// Std_ReturnType ecual_us_sensor_init(const us_sensor_config_t* us_sensor) {
//     Std_ReturnType ret = E_OK;
//     if (NULL == us_sensor) {
//         ret = E_NOT_OK;
//     }
//     else {
//         pin_change_interrupt_config_t pin_change_interrupt = {
//             .pin = us_sensor->echo_pin,
//             .pin_change_callback_interrupt_function = &us_sensor_app_interrupt_callback
//         };
//         ret = mcal_interrupt_pin_change_init(&pin_change_interrupt);
//         ret = mcal_gpio_pin_init(&(us_sensor->trigger_pin));
        
//         timer0_config_t timer0_local = {
//             .clock_source = TIMER0_1024_PRESCALE,
//             .timer0_interrupt = 1,
//             .timer0_callback_interrupt_function = &timer0_app_interrupt_callback
//         };
//         ret = mcal_timer0_init(&timer0_local);
//     }
//     return ret;
// }

// Std_ReturnType ecual_us_sensor_trig(const us_sensor_config_t* us_sensor) {
//     Std_ReturnType ret = E_OK;
//     if (NULL == us_sensor) {
//         ret = E_NOT_OK;
//     }
//     else {
//         ret = mcal_gpio_pin_logic_write(&(us_sensor->trigger_pin), HIGH);
//         _delay_us(10);
//         ret = mcal_gpio_pin_logic_write(&(us_sensor->trigger_pin), LOW);
//     }
//     return ret;
// }

// Std_ReturnType ecual_us_sensor_calc_distant(const us_sensor_config_t* us_sensor, uint16_t* distance) {
//     Std_ReturnType ret = E_OK;
//     if (NULL == us_sensor) {
//         ret = E_NOT_OK;
//     }
//     else {
//         float count_time = count_time = 1024 / (float)16000000;
//         if (0 != timer0_reg_value) {
//             *distance = (uint16_t)(timer0_reg_value * count_time * 1000000) / (float)58;
//         }
//         else {
// //            *distance = 0;
//         }
//     }
//     return ret;
// }

// Std_ReturnType ecual_us_sensor_invoke_on_range(const us_sensor_config_t* us_sensor, const uint16_t range_start, const uint16_t range_end, 
//                                                void (*true_function_to_invoke)(void), void (*false_function_to_invoke)(void)) {
//     Std_ReturnType ret = E_OK;
//     if((NULL == us_sensor) || (NULL == true_function_to_invoke) || (NULL == false_function_to_invoke) ) {
//         ret = E_NOT_OK;
//     }
//     else {
//         static uint16_t distance = 0;
//         ecual_us_sensor_trig(us_sensor);
//         _delay_ms(60);
//         ecual_us_sensor_calc_distant(us_sensor, &distance);
//         if ((range_start <= distance) && (range_end >= distance)) {
//            true_function_to_invoke();
//         }
//         else {
//            false_function_to_invoke();
//         }
//     }
// }

// void us_sensor_app_interrupt_callback(void) { 
//     static uint8_t rising_edge = 1; 
//     if (rising_edge == 1) {
//         TCNT0 = 0x00;
//         rising_edge = 0;
//         timer0_reg_value = 0;
//         TIMER0_INTERRUPT_ENABLE();
//     }
//     else {
//         timer0_reg_value = TCNT0 ; 
//         num_of_overflows = 0;
//         rising_edge = 1;
//         TIMER0_INTERRUPT_DISABLE();
//     }
// }

// void timer0_app_interrupt_callback(void) {
//     num_of_overflows++;
// }

