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

/* Global Variables --------------------------  -----------------------------*/

/* Initialization Function --------------------------------------------------*/ 
// This function will be called only once, suitable for initialization, oled, keypad, etc..
Std_ReturnType application_initialize(void) {
    Std_ReturnType ret = E_OK;
    return ret;
}

/* Functions Implementations ------------------------------------------------*/
// Default pass is "00000", config it if needed
void system_config(uint8_t** password, uint8_t* allowed_access_attempts) {
    *password = "00000";
    *allowed_access_attempts = 3;
}

// This function determine how the input will be taken, put the value in "received_digit" pointer;
// The '\r' will be the indicator that the the pass digits are complete, put it in "received_digit" when required.
Std_ReturnType take_digit_from_user(uint8_t* received_digit, const uint8_t digit_num) {
    Std_ReturnType ret = E_OK;
    return ret;   
}

Std_ReturnType correct_pass_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    // Action
    return ret;
}

Std_ReturnType incorrect_pass_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    // Action
    return ret;
}

// This function must have its own while loop with its breaker condition 
// When break from the loop, the system will reset and enable receiving from input again
Std_ReturnType limit_exceeded_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    uint8_t keep_in_loop = 1;
    while (keep_in_loop) {
     // Condition to break the loop
    }
    return ret;
}

