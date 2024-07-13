 /**
 ******************************************************************************
 * @file       ecual_led.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for led interfaces
 * @date       2024-3-15
 ******************************************************************************
*/
#include "ecual_led.h"

/* Global Variables ---------------------------------------------------------*/

/* Functions Implementations ------------------------------------------------*/
Std_ReturnType ecual_rgb_led_init(const rgb_led_config_t* rgb_led) {
    Std_ReturnType ret = E_OK;
    if(NULL == rgb_led) {
        ret = E_NOT_OK;
    }
    else {
        ret |= mcal_gpio_pin_direction_write(&(rgb_led->red_location), OUTPUT); 
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->red_location), LOW);

        ret |= mcal_gpio_pin_direction_write(&(rgb_led->blue_location), OUTPUT); 
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->blue_location), LOW);

        ret |= mcal_gpio_pin_direction_write(&(rgb_led->green_location), OUTPUT); 
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->green_location), LOW);
    }
    return ret;
}

Std_ReturnType ecual_rgb_led_turn_on(const rgb_led_config_t* rgb_led, const rgp_led_color_t rgp_led_color) {
    Std_ReturnType ret = E_OK;
    if(NULL == rgb_led) {
        ret = E_NOT_OK;
    }
    else {
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->red_location), LOW);
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->blue_location), LOW);
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->green_location), LOW);
        switch (rgp_led_color) {
        case RGP_LED_RED:
            ret |= mcal_gpio_pin_logic_write(&(rgb_led->red_location), HIGH);
            break;
        case RGP_LED_BLUE:
            ret |= mcal_gpio_pin_logic_write(&(rgb_led->blue_location), HIGH);
            break;
        case RGP_LED_GREEN:
            ret |= mcal_gpio_pin_logic_write(&(rgb_led->green_location), HIGH);
            break;
        case RGP_LED_YELLOW:
            ret |= mcal_gpio_pin_logic_write(&(rgb_led->red_location), HIGH);
            ret |= mcal_gpio_pin_logic_write(&(rgb_led->green_location), HIGH);
            break;
        default:
            break;
        }
    }
    return ret;
}

Std_ReturnType ecual_rgb_led_turn_off(const rgb_led_config_t* rgb_led) {
    Std_ReturnType ret = E_OK;
    if(NULL == rgb_led) {
        ret = E_NOT_OK;
    }
    else {
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->red_location), LOW);
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->blue_location), LOW);
        ret |= mcal_gpio_pin_logic_write(&(rgb_led->green_location), LOW);
    }
    return ret;
}

