/**
 ******************************************************************************
 * @mainpage   Security System
 * @brief      This source file contains the main function of the application
 * @date       2024-3-27
 ******************************************************************************
*/
#include "pass_auth_system_main.h"

/* Macro Declarations -------------------------------------------------------*/

/* Variables Definitions ----------------------------------------------------*/ 

/* Main Function ------------------------------------------------------------*/
int main(void) {
    Std_ReturnType ret = E_OK;
    ret |= application_initialize();
    uint8_t *password = NULL, *received_password;
    uint8_t allowed_access_attempts = 0, received_digit = 0, pass_overflow = 0;
    system_config(&password, &allowed_access_attempts);

    uint8_t remaining_access_attempts = allowed_access_attempts, digit_num = 0;
    received_password = (uint8_t *)malloc(strlen(password) + 1);
    received_password[strlen(password)] = '\0';
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
            else {
                if (received_digit) {
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
                    }
                    else {
                        pass_overflow = 1;
                    }
                    digit_num++;
                    received_digit = 0;
                }
                else {
                    /* Nothing */
                }
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
    
