/**
 ******************************************************************************
 * @mainpage   Password authentication system
 * @brief      This source file contains the main function of the application
 * @date       2024-3-27
 ******************************************************************************
*/
#include "pass_auth_system_config.h"

/* Macro Declarations -------------------------------------------------------*/
#define CHARACTER_WIDTH      8
#define PASSWORD_LENGTH      strlen(PASSWORD)
#define DISPLAY_START_COLUMN (((128 - (CHARACTER_WIDTH * PASSWORD_LENGTH))) / 2)

/* Variables Definitions ----------------------------------------------------*/ 

/* Main Function ------------------------------------------------------------*/
int main(void) {
    Std_ReturnType ret = E_OK;
    ret |= application_initialize();

    uint8_t password[] = PASSWORD, remaining_chars = PASSWORD_LENGTH, received_password[PASSWORD_LENGTH + 1],
    received_char = 0, access_attempts = NUMBER_OF_ALLOWED_ATTEMPTS , start_column = DISPLAY_START_COLUMN, clear = 1;

    ret |= ecual_oled_display_bit_mapping(&oled0, epd_bitmap_lock, 128, 64, 0, 0);    
    // Do forever
    while(1) {
        // If there are allowed attempts
        if (access_attempts > 0) { 
            // If the required password digits received
            if (0 == remaining_chars) {
                ret |= ecual_oled_display_clear(&oled0);
                // If received password is correct :
                if (!compare_strings(received_password, password)) {
                    access_attempts = NUMBER_OF_ALLOWED_ATTEMPTS;
                    ret |= correct_pass_callback_fun();
                }
                // If received password is incorrect :
                else {
                    access_attempts--;
                    ret |= incorrect_pass_callback_fun();
                }
                remaining_chars = PASSWORD_LENGTH;
                clear = 1;
                start_column = DISPLAY_START_COLUMN;
            }
            else {
#if defined(KEYPAD_USER_INPUT)
            	ret |= ecual_keypad_char_read(&keypad0, &received_char);
#elif defined(SERIAL_TERMINAL_USER_INPUT)
            	ret |= mcal_usart_receive_data(&received_char);
#else
/* Nothing */
#endif
                if (received_char) {
                    if (1 == clear) {
                        ret |= ecual_oled_display_clear(&oled0);
                        clear = 0;
                    }
                    else {
                        /* Nothing */
                    }
                    ret |= ecual_oled_display_char_write(&oled0, received_char, CHARACTER_WIDTH, 16,  2, start_column);
                    received_password[PASSWORD_LENGTH - remaining_chars] = received_char;
                    start_column += CHARACTER_WIDTH;
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
            access_attempts = NUMBER_OF_ALLOWED_ATTEMPTS;
	    }
    }
    return 0;
}
    
