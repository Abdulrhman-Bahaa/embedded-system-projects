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
#include "std/mcal_std_types.h"
#include <string.h>
#include <stdlib.h>

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType application_initialize(void);
void system_config(uint8_t** password, uint8_t* allowed_access_attempts);
Std_ReturnType take_digit_from_user(uint8_t* received_digit, const uint8_t digit_num);
Std_ReturnType correct_pass_callback_fun(const uint8_t* received_password);
Std_ReturnType incorrect_pass_callback_fun(const uint8_t* received_password);
// This function must have its own while loop with its breaker condition 
Std_ReturnType limit_exceeded_callback_fun(void);

#endif /* PASS_AUTH_SYSTEM_H */
