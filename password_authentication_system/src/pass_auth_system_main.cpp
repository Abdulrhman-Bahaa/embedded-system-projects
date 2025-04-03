/**
 ******************************************************************************
 * @mainpage   Security System
 * @brief      This source file contains the main function of the application
 * @date       2024-3-27
 ******************************************************************************
*/
#include "pass_auth_system_main.h"

/* Variables Definitions ----------------------------------------------------*/ 
static uint8_t* password = NULL;
static uint8_t received_password[MAX_PASS_DIGITS] = {'\0'}, allowed_access_attempts = 0;

/* Main Function ------------------------------------------------------------*/
int main(void) {
    Std_ReturnType ret = E_OK;
    ret |= application_initialize();
    ret |= system_config();
    uint8_t remaining_access_attempts = allowed_access_attempts, digit_num = 0, 
    received_digit = 0, pass_overflow = 0;
    // Do forever
    while(1) {
        // If there are allowed attempts
        if (remaining_access_attempts > 0) { 
            ret |= take_digit_from_user(&received_digit, digit_num);
            // If the required password digits received
            // Indicator : Enter (CR)
            if (13 == received_digit) {
                // If received password is correct :
                if ((!strcmp(received_password, password)) && (0 == pass_overflow)) {
                    remaining_access_attempts = allowed_access_attempts;
                    ret |= correct_pass_callback_fun();
                }
                // If received password is incorrect :
                else {
                    remaining_access_attempts--;
                    ret |= incorrect_pass_callback_fun();
                }
                pass_overflow = 0;
                digit_num = 0;
                received_digit = 0;
            }
            else if (received_digit){         
                // Backspace
                if (8 == received_digit) {
                    if (0 != digit_num) {
                        digit_num--;
                        if (digit_num < strlen(password)) {
                            pass_overflow = 0;
                        }
                    }
                }
                else if (digit_num < strlen(password)) {
                    received_password[digit_num] = received_digit;
                    digit_num++;
                }
                else {
                    pass_overflow = 1;
                }
                received_digit = 0;
            }
            else {
                /* Nothing */
            }
        }
        // If there are no allowed attempts
        else {
            // This function must have its own while loop with its breaker condition 
            ret |= limit_exceeded_callback_fun();
            remaining_access_attempts = allowed_access_attempts;
	    }
    }
    return 0;
}

/* Functions Implementations ------------------------------------------------*/
Std_ReturnType system_config(void) {
    Std_ReturnType ret = E_OK;
    uint8_t counter = 0, received_digit = 0, digit_num = 0, second_pass[MAX_PASS_DIGITS] = {'\0'}, 
    attempts_num = 0;
    static uint8_t first_pass[MAX_PASS_DIGITS] = {'\0'};
    while (1) {
        ret |= take_digit_from_user(&received_digit, digit_num);
        // If the required password digits received
        // Indicator : Enter (CR)
        if (13 == received_digit) {
            // If received password is correct :
            if (1 == counter) {
                if (!strcmp(first_pass, second_pass)) { 
                    password = first_pass;
                    ret |= second_pass_matched_callback_fun();
                    counter++;
                }
                // If received password does not match the first one :
                else {
                    ret |= second_pass_mismatched_callback_fun();
                    counter = 0;
                    first_pass[digit_num] = '\0';
                    second_pass[digit_num] = '\0';
                }
            }
            else if (0 == counter){
                counter++;
                ret |= first_pass_received_callback_fun();
            }
            else {
                allowed_access_attempts = attempts_num - '0';
                ret |= allowed_access_attempts_received_callback_fun();
                break;
            }
            digit_num = 0;
            received_digit = 0;
        }
        else if (received_digit) {
            // Backspace
            if (8 == received_digit) {
                if (0 != digit_num) {
                    digit_num--;
                }
            }
            else if (digit_num < MAX_PASS_DIGITS) {
                if (1 == counter) {
                    second_pass[digit_num] = received_digit;
                }
                else if (0 == counter) {
                    first_pass[digit_num] = received_digit;
                }
                else {
                    attempts_num = received_digit;
                }
                digit_num++;
            }
            else {
                /* Nothing */
            }
            received_digit = 0;
        }
        else {
            /* Nothing */
        }
    }
    return ret;
}
