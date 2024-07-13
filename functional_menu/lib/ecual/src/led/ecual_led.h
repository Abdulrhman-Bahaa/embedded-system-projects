 /**
 ******************************************************************************
 * @file       ecual_led.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for led interfaces
 * @date       2024-3-15
 ******************************************************************************
*/
#ifndef ECUAL_LED_H
#define	ECUAL_LED_H

/* Includes -----------------------------------------------------------------*/
#include <gpio/mcal_gpio.h>

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
typedef enum {
    RGP_LED_RED = 0,
    RGP_LED_BLUE,
    RGP_LED_GREEN,
    RGP_LED_YELLOW
}rgp_led_color_t;

typedef struct {
    pin_location_t red_location;
    pin_location_t blue_location;
    pin_location_t green_location;
}rgb_led_config_t;

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType ecual_rgb_led_init(const rgb_led_config_t* rgb_led);
Std_ReturnType ecual_rgb_led_turn_on(const rgb_led_config_t* rgb_led, const rgp_led_color_t rgp_led_color);
Std_ReturnType ecual_rgb_led_turn_off(const rgb_led_config_t* rgb_led);

#endif /* ECUAL_LED_H */

