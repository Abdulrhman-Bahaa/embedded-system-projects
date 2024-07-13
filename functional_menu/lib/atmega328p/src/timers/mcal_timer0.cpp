/**
 ******************************************************************************
 * @file       mcal_timer0.c
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes implementations for TIMER0 module
 * @date       2024-2-12
 ******************************************************************************
*/
#include "mcal_timer0.h"

/* Variables Definitions -----------------------------------------------------*/

/* Functions Implementations -------------------------------------------------*/
Std_ReturnType  mcal_timer0_init(timer0_config_t* timer0){
    Std_ReturnType ret = E_OK;
    if (NULL == timer0) {
       ret = E_NOT_OK;
    }
    else {
        CLEAR_BIT(TCCR0B, CS00);
        CLEAR_BIT(TCCR0B, CS01);
        CLEAR_BIT(TCCR0B, CS02);    
        switch (timer0->clock_source) {
             case TIMER0_NO_CLOCK_SOURCE:
                 // Timer/Counter0 stopped
                 break;
             case TIMER0_CLOCK_WITHOUT_PRESCALING:
                 // F_CPU (no prescaling)
                 SET_BIT(TCCR0B, CS00);
                 break;
             case TIMER0_8_PRESCALE:
                 // F_CPU / 8
                 SET_BIT(TCCR0B, CS01);
                 break;
             case TIMER0_64_PRESCALE:
                 // F_CPU / 64
                 SET_BIT(TCCR0B, CS00);
                 SET_BIT(TCCR0B, CS01);
                 break;
             case TIMER0_256_PRESCALE:
                 // F_CPU / 256
                 SET_BIT(TCCR0B, CS02);
                 break;
             case TIMER0_1024_PRESCALE:
                 // F_CPU / 1024
                 SET_BIT(TCCR0B, CS00);
                 SET_BIT(TCCR0B, CS02);
                 break;
             case TIMER0_EXTERNAL_CLOCK_SOURCE_ON_FALLING_EDGE:
                 // External clock source on falling edge
                 SET_BIT(TCCR0B, CS02);
                 SET_BIT(TCCR0B, CS01);
                 break;
             case TIMER0_EXTERNAL_CLOCK_SOURCE_ON_RISING_EDGE:
                 // External clock source on rising edge
                 SET_BIT(TCCR0B, CS00);
                 SET_BIT(TCCR0B, CS02);
                 SET_BIT(TCCR0B, CS01);
                 break;
            default:
                ret = E_NOT_OK;
        }
        if (STD_ON == timer0->timer0_interrupt){
            ret = mcal_interrupt_timer_init(TIMER0_INDEX, timer0->timer0_callback_interrupt_function);
        }
        else {
            TIMER0_INTERRUPT_DISABLE();
        }
        CLEAR_BIT(TCCR0A, WGM00);
        CLEAR_BIT(TCCR0A, WGM01);
        CLEAR_BIT(TCCR0B, WGM02);
        TCNT0 = 0x00;
    }
    return ret;   
}
