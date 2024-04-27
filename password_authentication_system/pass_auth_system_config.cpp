 /**
 ******************************************************************************
 * @file       pass_auth_system_config.cpp
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for led interfaces
 * @date       2024-4-27
 ******************************************************************************
*/
#include "pass_auth_system_config.h"

/* Global Variables ---------------------------------------------------------*/
const keypad_config_t keypad0 = {
    .columns = {
        {PORTB_INDEX, PIN2_INDEX},
        {PORTB_INDEX, PIN3_INDEX},
        {PORTB_INDEX, PIN4_INDEX}
    },
    .rows = {
        {PORTD_INDEX, PIN6_INDEX},
        {PORTD_INDEX, PIN7_INDEX},
        {PORTB_INDEX, PIN0_INDEX},
        {PORTB_INDEX, PIN1_INDEX}
    }
};

const usart_config_t usart0 = {
    .mode = USART_ASYNCHRONOUS_MODE,
    .ubrr_value = 16,
    .device_state = USART_TRANSMITTER_RECEIVER,
    .usart_interrupt = 0,
    .usart_receive_callback_interrupt_function = NULL
};

const oled_display_config_t oled0 = {
    .oled_display_address = 0x78
};

/* Initialization Function --------------------------------------------------*/ 
Std_ReturnType application_initialize(void) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_keypad_init(&keypad0);
    ret |= mcal_usart_init(&usart0);
    ret |= ecual_oled_display_init(&oled0);
    return ret;
}

/* Functions Implementations ------------------------------------------------*/
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
Std_ReturnType limit_exceeded_callback_fun(void) {
    Std_ReturnType ret = E_OK;
    // Action
    while (1) {
    }
    return ret;
}

