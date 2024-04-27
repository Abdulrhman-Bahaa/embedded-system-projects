 /**
 ******************************************************************************
 * @file       pass_auth_system_config.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for led interfaces
 * @date       2024-4-27
 ******************************************************************************
*/
#ifndef PASS_AUTH_SYSTEM_CONFIG_H
#define	PASS_AUTH_SYSTEM_CONFIG_H

/* Includes -----------------------------------------------------------------*/
#include "keypad/ecual_keypad.h"
#include <usart/mcal_usart.h>
#include "oled_display/ecual_oled_display.h"

/* Macro Declarations -------------------------------------------------------*/
#define NUMBER_OF_ALLOWED_ATTEMPTS  2
#define PASSWORD                    "000000"    // 10 digits max
#define KEYPAD_USER_INPUT           1
// #define SERIAL_TERMINAL_USER_INPUT  1

/* Macro Functions Declarations ---------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/
extern const keypad_config_t keypad0;
extern const usart_config_t usart0;
extern const oled_display_config_t oled0;

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType application_initialize(void);
Std_ReturnType correct_pass_callback_fun(void);
Std_ReturnType incorrect_pass_callback_fun(void);
// This function must have its own while loop with its breaker condition 
Std_ReturnType limit_exceeded_callback_fun(void);

#endif /* PASS_AUTH_SYSTEM_H */

