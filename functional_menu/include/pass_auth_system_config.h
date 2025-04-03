 /**
 ******************************************************************************
 * @file       pass_auth_system_config.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for led interfaces
 * @date       2024-5-2
 ******************************************************************************
*/
#ifndef PASS_AUTH_SYSTEM_CONFIG_H
#define	PASS_AUTH_SYSTEM_CONFIG_H

/* Includes -----------------------------------------------------------------*/
#include "keypad/ecual_keypad.h"
#include "usart/mcal_usart.h"
#include "wifi_module/ecual_wifi_module.h"
#include "oled_display/ecual_oled_display.h"
#include "led/ecual_led.h"
#include "OLED_menu.h"

/* Macro Declarations -------------------------------------------------------*/
#define CHARACTER_WIDTH      8
#define DISPLAY_START_COLUMN (((128 - (CHARACTER_WIDTH * 5))) / 2)
// #define KEYPAD_USER_INPUT           1
#define SERIAL_TERMINAL_USER_INPUT  1

/* Macro Functions Declarations ---------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/

#endif /* PASS_AUTH_SYSTEM_CONFIG_H */

