/**
 ******************************************************************************
 * @file       mcal_external_interrupts.c
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains the implementations for external 
 *             interrupts interfaces
 * @date       2024-2-16
 ******************************************************************************
*/
#include "mcal_external_interrupts.h"

/* Variables Definitions -----------------------------------------------------*/
void (*pin_change_isr_app_ptr[3])(void) = {NULL};

/* Functions Implementations -------------------------------------------------*/
Std_ReturnType mcal_interrupt_pin_change_init(const pin_change_interrupt_config_t* pin_change_interrupt) {
    Std_ReturnType ret = E_OK;
    if (NULL == pin_change_interrupt) {
        ret = E_NOT_OK;
    }
    else {
        GLOBAL_INTERRUPT_DISABLE();
        port_index_t port;
        uint8_t interrupt_pin = (pin_change_interrupt->pin);
        if (interrupt_pin >= 0 && interrupt_pin <= 7) {
            port = PORTB_INDEX;
            SET_BIT(PCICR, PCIE0);
            SET_BIT(PCMSK0, interrupt_pin); 
            pin_change_isr_app_ptr[0] = pin_change_interrupt->pin_change_callback_interrupt_function;
        } else if (interrupt_pin >= 8 && interrupt_pin <= 14) {
            interrupt_pin -= 8;
            port = PORTC_INDEX;
            SET_BIT(PCICR, PCIE1);
            SET_BIT(PCMSK1, interrupt_pin);
            pin_change_isr_app_ptr[1] = pin_change_interrupt->pin_change_callback_interrupt_function;
        } else if (interrupt_pin >= 16 && interrupt_pin<= 23) {
            interrupt_pin -= 16;
            port = PORTD_INDEX;
            SET_BIT(PCICR, PCIE2);
            SET_BIT(PCMSK2, interrupt_pin);
            pin_change_isr_app_ptr[2] = pin_change_interrupt->pin_change_callback_interrupt_function;
        } else {
            ret = E_NOT_OK;
        }
        pin_location_t pin_location = {
            .port = port,
            .pin = interrupt_pin
        };
        ret |= mcal_gpio_pin_direction_write(&pin_location, INPUT);
        GLOBAL_INTERRUPT_ENABLE();
    }
    return ret;
}
