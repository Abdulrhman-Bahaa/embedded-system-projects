/**
 ******************************************************************************
 * @file       mcal_external_interrupts_isr.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations for external interrupts isr
 * @date       2024-2-16
 ******************************************************************************
*/
#ifndef MCAL_EXTERNAL_INTERRUPTS_ISR_H
#define	MCAL_EXTERNAL_INTERRUPTS_ISR_H

/* Includes -----------------------------------------------------------------*/
#include <avr/interrupt.h>
#include "mcal_external_interrupts.h"

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
void pin_change0_isr(void);
void pin_change1_isr(void);
void pin_change2_isr(void);

#endif	/* MCAL_EXTERNAL_INTERRUPTS_ISR_H */
