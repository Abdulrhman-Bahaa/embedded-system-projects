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
    uint8_t* password = NULL, *received_password = NULL:
    uint8_t allowed_access_attempts = 0, received_char = 0;
    system_config(&password, &allowed_access_attempts);

    uint8_t remaining_access_attempts = allowed_access_attempts, remaining_chars = strlen(password);
    received_password = (uint8_t*)malloc(strlen(password) + 1);
    received_password[strlen(password)] = '\0';
    // Do forever
    while(1) {
        // If there are allowed attempts
        if (remaining_access_attempts > 0) { 
            // If the required password digits received
            if (0 == remaining_chars) {
                ret |= pass_received_callback_fun(received_password);
                // If received password is correct :
                if (!strcmp(received_password, password)) {
                    remaining_access_attempts = allowed_access_attempts;
                    ret |= correct_pass_callback_fun();
                }
                // If received password is incorrect :
                else {
                    remaining_access_attempts--;
                    ret |= incorrect_pass_callback_fun();
                }
                remaining_chars = strlen(password);
            }
            else {
                ret |= user_input_fun(&received_char);
                if (received_char) {
                    ret |= receiving_chars_callback_fun(received_char);
                    received_password[strlen(password) - remaining_chars] = received_char;
                    remaining_chars--;
                    received_char = 0;
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
