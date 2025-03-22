/**
 ******************************************************************************
 * @file       mcal_std_types.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes a standard data types that be used in this project
 * @date       2024-2-09
 ******************************************************************************
*/
#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H

/* Includes -----------------------------------------------------------------*/
#include <avr/io.h>

/* Macro Declarations -------------------------------------------------------*/
#define BIT_MASK                        0x01
#define STD_HIGH                        0x01
#define STD_LOW                         0x00
#define STD_ON                          0x01
#define STD_OFF                         0x00
#define STD_ACTIVE                      0x01
#define STD_IDLE                        0x00
#define E_OK            (Std_ReturnType)0x01
#define E_NOT_OK        (Std_ReturnType)0x00
#define STD_ACTIVE                      0x01
#define STD_IDLE                        0x00
#define NULL                        (void*)0

/* Macro Functions Declarations ---------------------------------------------*/
#define SET_BIT(REG, BIT_POSN)               (REG |= (BIT_MASK <<  BIT_POSN))
#define CLEAR_BIT(REG, BIT_POSN)             (REG &= ~(BIT_MASK <<  BIT_POSN))
#define TOGGLE_BIT(REG, BIT_POSN)            (REG ^= (BIT_MASK <<  BIT_POSN))
#define READ_BIT(REG, BIT_POSN)              ((REG >> BIT_POSN) & BIT_MASK )

/* Data Type Declarations ---------------------------------------------------*/
typedef unsigned char    uint8_t;
typedef signed char      sint8_t;
typedef signed short     sint16_t;
typedef uint8_t          Std_ReturnType;

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/

#endif	/* MCAL_STD_TYPES_H */
