/**
 ******************************************************************************
 * @file       mcal_internal_interrupts.c
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains the implementations for char interrupts
 *             interfaces
 * @date       2023-12-10
 ******************************************************************************
*/
#include "mcal_internal_interrupts.h"

/* Variables Definitions -----------------------------------------------------*/
void (*timer_isr_app_ptr[3])(void) = {NULL};
void (*usart_receive_isr_app_ptr)(void) = {NULL};

/* Functions Implementations -------------------------------------------------*/
Std_ReturnType mcal_interrupt_timer_init(timer_index_t timer_number, void (*timer_callback_interrupt_function)(void)){
    Std_ReturnType ret = E_OK;
    if (NULL == timer_callback_interrupt_function) {
        ret = E_NOT_OK;
    }
    else {
        GLOBAL_INTERRUPT_DISABLE();
        switch(timer_number){
            case TIMER0_INDEX: 
                timer_isr_app_ptr[0] = timer_callback_interrupt_function;
                TIMER0_INTERRUPT_ENABLE();
                break;
            default:
                ret = E_NOT_OK; 
        }
        GLOBAL_INTERRUPT_ENABLE();
    }
    return ret;
}
