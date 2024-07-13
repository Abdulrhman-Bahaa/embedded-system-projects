/**
 ******************************************************************************
 * @file       mcal_external_interrupts.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations for external 
 *             interrupts module
 * @date       2024-2-16
 ******************************************************************************
*/
#ifndef MCAL_EXTERNAL_INTERRUPTS_H
#define	MCAL_EXTERNAL_INTERRUPTS_H

/* Includes -----------------------------------------------------------------*/
#include "../../std/mcal_std_types.h"
#include "../../gpio/mcal_gpio.h"

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/
#define GLOBAL_INTERRUPT_ENABLE()               (SET_BIT(SREG, 7))
#define GLOBAL_INTERRUPT_DISABLE()              (CLEAR_BIT(SREG, 7))

#define PIN_CHANGE0_INTERRUPT_FLAG_CLEAR()           (SET_BIT(PCIFR, PCIF0))
#define PIN_CHANGE1_INTERRUPT_FLAG_CLEAR()           (SET_BIT(PCIFR, PCIF1))
#define PIN_CHANGE2_INTERRUPT_FLAG_CLEAR()           (SET_BIT(PCIFR, PCIF2))

/* Data Type Declarations ---------------------------------------------------*/
typedef enum {
    INTERRUPT_PCINT0 = 0,
    INTERRUPT_PCINT1,
    INTERRUPT_PCINT2,
    INTERRUPT_PCINT3,
    INTERRUPT_PCINT4,
    INTERRUPT_PCINT5,
    INTERRUPT_PCINT6,
    INTERRUPT_PCINT7,
    INTERRUPT_PCINT8,
    INTERRUPT_PCINT9,
    INTERRUPT_PCINT10,
    INTERRUPT_PCINT11,
    INTERRUPT_PCINT12,
    INTERRUPT_PCINT13,
    INTERRUPT_PCINT14,
    INTERRUPT_RESERVED,
    INTERRUPT_PCINT16,
    INTERRUPT_PCINT17,
    INTERRUPT_PCINT18,
    INTERRUPT_PCINT19,
    INTERRUPT_PCINT20,
    INTERRUPT_PCINT21,
    INTERRUPT_PCINT22,
    INTERRUPT_PCINT23
}pin_change_interrupt_pins_t;

typedef struct {
    pin_change_interrupt_pins_t pin;
    void (*pin_change_callback_interrupt_function)(void);
}pin_change_interrupt_config_t;

/* Variables Declarations ---------------------------------------------------*/
extern void (*pin_change_isr_app_ptr[3])(void);

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType mcal_interrupt_pin_change_init(const pin_change_interrupt_config_t* pin_change_interrupt);

#endif	/* MCAL_INTERRUPT_H */
