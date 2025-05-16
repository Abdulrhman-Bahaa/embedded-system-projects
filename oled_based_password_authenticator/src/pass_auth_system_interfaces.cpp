 /**
 ******************************************************************************
 * @file       pass_auth_system_config.cpp
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for led interfaces
 * @date       2024-4-27
 ******************************************************************************
*/
/* Includes -----------------------------------------------------------------*/
#include "pass_auth_system_config.h"

/* Global Variables ---------------------------------------------------------*/
static const usart_config_t usart0 = {
    .mode = USART_ASYNCHRONOUS_MODE,
    .ubrr_value = 16,
    .device_state = USART_TRANSMITTER_RECEIVER,
    .usart_interrupt = 1,
    .usart_receive_callback_interrupt_function = NULL
};

const oled_display_config_t oled0 = {
    .oled_display_address = 0x78
};

static const uint8_t token[] = "BBUS-Ow18QcxUismkXPVYjDgUFKjStQqVeM";
uint8_t start_column = 0, pass_length = 0, clear = 1;

static const rgb_led_config_t rgb0 = {
    .red_location = {
        .port = PORTD_INDEX,
        .pin = PIN3_INDEX
    },
    .blue_location = {
        .port = PORTD_INDEX,
        .pin = PIN5_INDEX
    },
    .green_location = {
        .port = PORTD_INDEX,
        .pin = PIN4_INDEX
    }
};

static const pin_config_t buzzer0 = {
    .location = {
        .port = PORTB_INDEX,
        .pin = PIN5_INDEX
    },
    .direction = OUTPUT,
    .logic = LOW
};

/* Initialization Function --------------------------------------------------*/ 
Std_ReturnType application_initialize(void) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_keypad_init(&keypad0);
    ret |= ecual_oled_display_init(&oled0);
    ret |= ecual_rgb_led_init(&rgb0);
    ret |= mcal_gpio_pin_init(&buzzer0);    
    ret |= mcal_usart_init(&usart0);
    ret |= ecual_oled_display_clear(&oled0);
    ret |= ecual_oled_display_string_write(&oled0, "Enter pass", 8, 16, 0, 13);
    return ret;
}

/* Functions Implementations ------------------------------------------------*/
// This function determine how the input will be taken, put the value in "received_digit" pointer;
Std_ReturnType take_digit_from_user(uint8_t* received_digit, const uint8_t digit_num) {
    Std_ReturnType ret = E_OK;
    ret |= mcal_usart_receive_data(received_digit);
    if (*received_digit) {
        if (8 == *received_digit) {
            start_column -= CHARACTER_WIDTH;
            ret |= ecual_oled_display_char_write(&oled0, ' ', 8, 16,  2, start_column);
        }
        else if (13 == *received_digit){ 
        }
        else {
            start_column += CHARACTER_WIDTH;
            ret |= ecual_oled_display_char_write(&oled0, *received_digit, 8, 16,  2, start_column);
        }
    }
    else {
        /* Nothing */
    }
    return ret;   
}

Std_ReturnType first_pass_received_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_oled_display_clear(&oled0);
    start_column = 0;
    ret |= ecual_oled_display_string_write(&oled0, "Confirm pass", 8, 16, 0, 13);
    return ret;
}

Std_ReturnType second_pass_matched_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_oled_display_clear(&oled0);
    start_column = 0;
    ret |= ecual_oled_display_string_write(&oled0, "Ok", 8, 16, 0, 13);
    _delay_ms(1000);
    ret |= ecual_oled_display_string_write(&oled0, "Enter access num", 8, 16, 0, 13);
    return ret;
}

Std_ReturnType second_pass_mismatched_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_oled_display_clear(&oled0);
    start_column = 0;
    ret |= ecual_oled_display_string_write(&oled0, "Pass mismatch", 8, 16, 0, 13);
    _delay_ms(1000);
    return ret;
}

Std_ReturnType allowed_access_attempts_received_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    start_column = 0;
    ret |= ecual_oled_display_bit_mapping(&oled0, epd_bitmap_hamad, 128, 64, 0, 0);    
    return ret;
}

Std_ReturnType correct_pass_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    start_column = 0;
    ret = oled_menu_with_functionality();
    return ret;
}

Std_ReturnType incorrect_pass_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    // Action
    ret |= ecual_oled_display_clear(&oled0);
    start_column = 0;
    ret |= ecual_oled_display_string_write(&oled0, "Access denied", 8, 16, 0, 13);
}

// This function must have its own while loop with its breaker condition 
Std_ReturnType limit_exceeded_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_oled_display_string_write(&oled0, "No more attemptsallowed", 8, 16, 0, 0);
    _delay_ms(4000);
    ret |= ecual_oled_display_bit_mapping(&oled0, epd_bitmap_lock, 128, 64, 0, 0);
    while (1) {
    }
    return ret;
}

