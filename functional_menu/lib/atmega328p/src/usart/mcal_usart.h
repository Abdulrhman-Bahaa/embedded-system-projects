/**
 ******************************************************************************
 * @file       mcal_usart.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations for USART module
 *             The specific model used is PIC16F887.
 * @date       2024-1-31
 ******************************************************************************
*/
#ifndef MCAL_USART_H
#define	MCAL_USART_H

/* Includes -----------------------------------------------------------------*/
#include "../gpio/mcal_gpio.h"
#include "../interrupts/internal/mcal_internal_interrupts.h"

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

#define USART_SYNCHRONOUS_MODE()  (UCSR0C &= 0x3F)
#define USART_ASYNCHRONOUS_MODE() (UCSR0C &= 0x7F)


/* Data Type Declarations ---------------------------------------------------*/
typedef enum {
    USART_ASYNCHRONOUS_MODE = 0,
    USART_SYNCHRONOUS_MODE
}usart_mode_t;

typedef enum {
    USART_TRANSMITTER = 0,
    USART_RECEIVER,
    USART_TRANSMITTER_RECEIVER
}usart_device_state_t;

typedef enum {
    USART_8_BIT_DATA = 0,
    USART_9_BIT_DATA
}data_size_t;

typedef struct {
    usart_mode_t mode;
    uint16_t ubrr_value; 
    usart_device_state_t device_state;
    uint8_t usart_interrupt : 1;
    void (*usart_receive_callback_interrupt_function)(void);
}usart_config_t;

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType mcal_usart_init(const usart_config_t* usart);
Std_ReturnType mcal_usart_transmit_data(const uint8_t byte_to_transmit);
Std_ReturnType mcal_usart_receive_data(uint8_t* received_byte);
Std_ReturnType mcal_usart_transmit_string(const char string[]);

#endif	/* MCAL_USART_H */
