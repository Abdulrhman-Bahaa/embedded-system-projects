/**
 ******************************************************************************
 * @file       mcal_timer0.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations for timer0 module
 * @date       2024-2-12
 ******************************************************************************
*/
#ifndef MCAL_TIMER0_H
#define	MCAL_TIMER0_H

/* Includes -----------------------------------------------------------------*/
#include <std/mcal_std_types.h>
#include <interrupts/internal/mcal_internal_interrupts.h>

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
typedef enum {
    TIMER0_NO_CLOCK_SOURCE = 0,
    TIMER0_CLOCK_WITHOUT_PRESCALING,
    TIMER0_8_PRESCALE,
    TIMER0_64_PRESCALE,
    TIMER0_256_PRESCALE,
    TIMER0_1024_PRESCALE,
    TIMER0_EXTERNAL_CLOCK_SOURCE_ON_FALLING_EDGE,
    TIMER0_EXTERNAL_CLOCK_SOURCE_ON_RISING_EDGE        
}timer0_clock_source_t;

typedef struct {
    timer0_clock_source_t clock_source;
    uint8_t timer0_interrupt : 1;
    void (*timer0_callback_interrupt_function)(void);
}timer0_config_t;

/* Variables Declarations ---------------------------------------------------*/


/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType mcal_timer0_init(timer0_config_t* timer0);

#endif	/* MCAL_TIMER0_H */
