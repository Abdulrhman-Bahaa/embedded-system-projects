 /**
 ******************************************************************************
 * @file       ecual_keypad.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for keypad interfaces
 * @date       2024-3-15
 ******************************************************************************
*/
#ifndef ECUAL_KEYPAD_H
#define	ECUAL_KEYPAD_H

/* Includes -----------------------------------------------------------------*/
#include <gpio/mcal_gpio.h>
#include "ecual_keypad_configs.h"

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
typedef struct {
    pin_location_t columns[KEYPAD_COLUMNS_NUMBER];
    pin_location_t rows[KEYPAD_ROWS_NUMBER];
}keypad_config_t;

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType ecual_keypad_init(const keypad_config_t* keypad);
Std_ReturnType ecual_keypad_char_read(const keypad_config_t* keypad, uint8_t* received_char);

#endif /* ECUAL_KEYPAD_H */

