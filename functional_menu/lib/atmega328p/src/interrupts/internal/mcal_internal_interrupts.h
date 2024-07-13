/**
 ******************************************************************************
 * @file       mcal_internal_interrupts.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations for interrupts module
 * @date       2024-2-16
 ******************************************************************************
*/
#ifndef MCAL_INTERNAL_INTERRUPTS_H
#define	MCAL_INTERNAL_INTERRUPTS_H

/* Includes -----------------------------------------------------------------*/
#include "../../std/mcal_std_types.h"

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/
#define GLOBAL_INTERRUPT_ENABLE()               (SET_BIT(SREG, 7))
#define GLOBAL_INTERRUPT_DISABLE()              (CLEAR_BIT(SREG, 7))

#define TIMER0_INTERRUPT_ENABLE()               (SET_BIT(TIMSK0, TOIE0))
#define TIMER0_INTERRUPT_DISABLE()              (CLEAR_BIT(TIMSK0, TOIE0))
#define TIMER0_INTERRUPT_FLAG_CLEAR()           (SET_BIT(TIFR0, TOV0))

#define USART_RECEIVE_INTERRUPT_ENABLE()               (SET_BIT(UCSR0B, RXCIE0))
#define USART_RECEIVE_INTERRUPT_DISABLE()              (CLEAR_BIT(UCSR0B, RXCIE0))

/* Data Type Declarations ---------------------------------------------------*/
typedef enum {
    TIMER0_INDEX = 0,
    TIMER1_INDEX,
    TIMER2_INDEX
}timer_index_t;

/* Variables Declarations ---------------------------------------------------*/
extern void (*timer_isr_app_ptr[3])(void);
extern void (*usart_receive_isr_app_ptr)(void);

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType mcal_interrupt_timer_init(timer_index_t timer_number, void (*timerx_callback_interrupt_function)(void));

#endif	/* MCAL_INTERNAL_INTERRUPTS_H */
