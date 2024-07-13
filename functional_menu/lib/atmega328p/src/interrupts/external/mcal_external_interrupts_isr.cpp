/**
 ******************************************************************************
 * @file       mcal_external_interrupts_isr.c
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains the implementations for external 
 *             interrupts isr
 * @date       2024-2-16
 ******************************************************************************
*/
#include "mcal_external_interrupts_isr.h"

/* Variables Definitions -----------------------------------------------------*/
ISR(PCINT0_vect) {
    pin_change0_isr();
}

ISR(PCINT1_vect) {
    pin_change1_isr();
}

ISR(PCINT2_vect) {
    pin_change2_isr();
}

/* Functions Implementations -------------------------------------------------*/
void pin_change0_isr(void) {
    if (NULL != pin_change_isr_app_ptr[0]) {
       pin_change_isr_app_ptr[0]();
    }
    else {
        /* Nothing */
    }
    PIN_CHANGE0_INTERRUPT_FLAG_CLEAR();
}

void pin_change1_isr(void) {
    if (NULL != pin_change_isr_app_ptr[1]) {
       pin_change_isr_app_ptr[1]();
    }
    else {
        /* Nothing */
    }
    PIN_CHANGE1_INTERRUPT_FLAG_CLEAR();
}

void pin_change2_isr(void) {
    if (NULL != pin_change_isr_app_ptr[2]) {
       pin_change_isr_app_ptr[2]();
    }
    else {
        /* Nothing */
    }
    PIN_CHANGE2_INTERRUPT_FLAG_CLEAR();
}

