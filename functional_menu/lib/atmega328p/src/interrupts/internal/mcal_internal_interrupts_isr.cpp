/**
 ******************************************************************************
 * @file       mcal_internal_interrupts_isr.c
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains the implementations for char interrupts
 *             interfaces
 * @date       2023-12-10
 ******************************************************************************
*/
#include "mcal_internal_interrupts_isr.h"

/* Variables Definitions -----------------------------------------------------*/

/* Functions Implementations -------------------------------------------------*/
ISR(TIMER0_OVF_vect) {
    timer0_isr();
}

ISR(USART_RX_vect) {
    usart_receive_isr();
}


void timer0_isr(void){
    if (NULL != timer_isr_app_ptr[0]) {
       timer_isr_app_ptr[0]();
    }
    else {
        /* Nothing */
    }
    TIMER0_INTERRUPT_FLAG_CLEAR();
}

void usart_receive_isr(void) {
    if (NULL != usart_receive_isr_app_ptr) {
       usart_receive_isr_app_ptr();
    }
    else {
        /* Nothing */
    }
}
