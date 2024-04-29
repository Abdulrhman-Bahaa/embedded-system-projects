 /**
 ******************************************************************************
 * @file       pass_auth_system_config.cpp
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for led interfaces
 * @date       2024-4-27
 ******************************************************************************
*/
/* Includes -----------------------------------------------------------------*/
#include "std/mcal_std_types.h"

/* Macro Declarations -------------------------------------------------------*/

/* Global Variables ---------------------------------------------------------*/

/* Initialization Function --------------------------------------------------*/ 
Std_ReturnType application_initialize(void) {
    Std_ReturnType ret = E_OK;  
    return ret;
}

/* Functions Implementations ------------------------------------------------*/
void system_config(uint8_t** password, uint8_t* allowed_access_attempts) {
    *password = "Write pass here";
    *allowed_access_attempts = 1;
}

// This function determine how the input will be taken, put the value in "received_char" pointer;
Std_ReturnType take_digit_from_user(uint8_t* received_digit) {
    Std_ReturnType ret = E_OK;
    return ret;   
}

Std_ReturnType pass_received_callback_fun(const uint8_t* received_password) {
    Std_ReturnType ret = E_OK;
    return ret;
}   

Std_ReturnType correct_pass_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    return ret;
}

Std_ReturnType incorrect_pass_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    // Action
    return ret;
}

// This function must have its own while loop with its breaker condition 
Std_ReturnType limit_exceeded_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    uint8_t keep_in_loop = 1;
    while (keep_in_loop) {
      // Condition to break the loop
    }
    return ret;
}
