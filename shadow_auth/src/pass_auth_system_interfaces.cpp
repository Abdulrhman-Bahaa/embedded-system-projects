 /**
 ******************************************************************************
 * @file       pass_auth_system_config.cpp
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for led interfaces
 * @date       2024-4-27
 ******************************************************************************
*/
/* Includes -----------------------------------------------------------------*/
#include "pass_auth_system_main.h"

/* Global Variables ---------------------------------------------------------*/

/* Initialization Function --------------------------------------------------*/ 
Std_ReturnType application_initialize(void) {
    Std_ReturnType ret = E_OK;
    return ret;
}

/* Functions Implementations ------------------------------------------------*/
// This function determine how the input will be taken, put the value in "received_digit" pointer.
// The '\r' will be the indicator that the the pass digits are complete, put it in "received_digit" when required.
// The 'Backspace'(8 in ASCII) will be the indicator that removing the current digit is required, put it in "received_digit" when required.
// when backspace is received (delete digit), the new digit will be put in the right digit automatically, leaving "digit_num" with
// the correct value.
Std_ReturnType take_digit_from_user(uint8_t* received_digit, const uint8_t digit_num) {
    Std_ReturnType ret = E_OK;
    return ret;   
}

Std_ReturnType first_pass_received_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    // Action
    return ret;
}

Std_ReturnType second_pass_matched_callback_fun(void) {
    Std_ReturnType ret = E_OK;
     // Action
    return ret;
}

Std_ReturnType second_pass_mismatched_callback_fun(void) {
    Std_ReturnType ret = E_OK;
     // Action
    return ret;
}

Std_ReturnType allowed_access_attempts_received_callback_fun(void) {
    Std_ReturnType ret = E_OK;
     // Action
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

// This function will be called if "allowed_access_attempts" are consumed.
// This function must have its own while loop with its breaker condition.
// When break from the loop, the system will reset and enable receiving from input again.
Std_ReturnType limit_exceeded_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    while (1) {
    }
    return ret;
}
