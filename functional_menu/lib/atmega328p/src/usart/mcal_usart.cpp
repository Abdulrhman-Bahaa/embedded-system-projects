/**
 ******************************************************************************
 * @file       mcal_usart.c
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes implementations for USART module
 *             The specific model used is PIC16F887.
 * @date       2024-1-31
 ******************************************************************************
*/
#include "mcal_usart.h"

void mcal_usart_transmit_callback_fun(void);

/* Variables Definitions -----------------------------------------------------*/
volatile uint8_t received_byte_globe = 0;

/* Functions Implementations -------------------------------------------------*/
Std_ReturnType mcal_usart_init(const usart_config_t* usart) {
    Std_ReturnType ret = E_OK;
    if(NULL == usart){
        ret = E_NOT_OK;
    }
    else {        
        // USART mode set
        if (USART_ASYNCHRONOUS_MODE == (usart->mode)) {
            USART_ASYNCHRONOUS_MODE();
        }
        else {
            USART_SYNCHRONOUS_MODE();
        }
        // Set baud rate
        // Set baud rate divider to 16
        SET_BIT(UCSR0A, U2X0);
        UBRR0H = (uint8_t)((usart->ubrr_value) >> 8);
        UBRR0L = (uint8_t)(usart->ubrr_value);
        // Enable receiver and transmitter
        switch(usart->device_state) {
            case USART_TRANSMITTER:
                SET_BIT(UCSR0B, TXEN0);
                break;
            case USART_RECEIVER:
                SET_BIT(UCSR0B, RXEN0);
                break;
            case USART_TRANSMITTER_RECEIVER:
                SET_BIT(UCSR0B, TXEN0);
                SET_BIT(UCSR0B, RXEN0);
                break;
            default:
                ret |= E_NOT_OK;
        }
        if (usart->usart_interrupt) {
            usart_receive_isr_app_ptr = &mcal_usart_transmit_callback_fun;
             USART_RECEIVE_INTERRUPT_ENABLE();
             GLOBAL_INTERRUPT_ENABLE();
        }
        else {
             USART_RECEIVE_INTERRUPT_DISABLE();
        }
        // Set frame format: 8data, 1 stop bit
        SET_BIT(UCSR0C, UCSZ00);
        SET_BIT(UCSR0C, UCSZ01);
        CLEAR_BIT(UCSR0B, UCSZ02);
    }
    return ret; 
}


Std_ReturnType mcal_usart_transmit_data(const uint8_t byte_to_transmit) {
    Std_ReturnType ret = E_OK;
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = byte_to_transmit;
    return ret; 
}

Std_ReturnType mcal_usart_receive_data(uint8_t* received_byte) {
    Std_ReturnType ret = E_OK;
    if(NULL == received_byte) {
        ret = E_NOT_OK;
    }
    else {        
        // /* Wait for data to be received */
        // while (!(UCSR0A & (1<<RXC0)));
        // /* Get and return received data from buffer */
        // *received_byte = UDR0;
        if (0 != received_byte_globe) {
            *received_byte = received_byte_globe;
            received_byte_globe = 0;
        }
    }
    return ret; 
}

Std_ReturnType mcal_usart_transmit_string(const char string[]) {
    Std_ReturnType ret = E_OK;   
    uint8_t i = 0;
    while('\0' != string[i]) {
        ret = mcal_usart_transmit_data((uint8_t)string[i]);
        i++;
    }
    return ret; 
}

void mcal_usart_transmit_callback_fun(void) {
    received_byte_globe = UDR0;
}

