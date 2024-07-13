/**
 ******************************************************************************
 * @file       mcal_internal_interrupts_isr.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations for interrupts module
 * @date       2023-12-10
 ******************************************************************************
*/
#ifndef MCAL_INTERNAL_INTERRUPTS_ISR_H
#define	MCAL_INTERNAL_INTERRUPTS_ISR_H

/* Includes -----------------------------------------------------------------*/
#include <avr/interrupt.h>
#include "mcal_internal_interrupts.h"

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
void timer0_isr(void);
void usart_receive_isr(void);

#endif	/* MCAL_INTERNAL_INTERRUPTS_ISR_H */
