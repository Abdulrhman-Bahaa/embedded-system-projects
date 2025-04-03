 /**
 ******************************************************************************
 * @file       pass_auth_system_main.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for led interfaces
 * @date       2024-4-27
 ******************************************************************************
*/
#ifndef PASS_AUTH_SYSTEM_CONFIG_H
#define	PASS_AUTH_SYSTEM_CONFIG_H

/* Includes -----------------------------------------------------------------*/
#include "std/mcal_std_types.h"
#include <string.h>

/* Macro Declarations -------------------------------------------------------*/
#define MAX_PASS_DIGITS 20

/* Macro Functions Declarations ---------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType application_initialize(void);
Std_ReturnType take_digit_from_user(uint8_t* received_digit, const uint8_t digit_num);
Std_ReturnType system_config(void);
Std_ReturnType first_pass_received_callback_fun(void);
Std_ReturnType second_pass_matched_callback_fun(void);
Std_ReturnType second_pass_mismatched_callback_fun(void);
Std_ReturnType allowed_access_attempts_received_callback_fun(void);
Std_ReturnType correct_pass_callback_fun(void);
Std_ReturnType incorrect_pass_callback_fun(void);
// This function must have its own while loop with its breaker condition 
Std_ReturnType limit_exceeded_callback_fun(void);

#endif /* PASS_AUTH_SYSTEM_H */

