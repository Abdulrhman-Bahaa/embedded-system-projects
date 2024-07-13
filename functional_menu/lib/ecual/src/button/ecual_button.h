 /**
 ******************************************************************************
 * @file       ecual_button.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for button 
 *             interfaces
 * @date       2023-9-06
 ******************************************************************************
*/
#ifndef ECUAL_BUTTON_H
#define	ECUAL_BUTTON_H

/* Includes -----------------------------------------------------------------*/
#include <gpio/mcal_gpio.h>

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
}button_status_t;

typedef enum {
    ACTIVE_LOW = 0,
    ACTIVE_HIGH
}button_active_t;

typedef struct {
    pin_location_t location;     
    button_active_t connection;
}button_config_t;

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType ecual_button_init(const button_config_t* btn);
Std_ReturnType ecual_button_read(const button_config_t* btn, button_status_t* btn_status);
Std_ReturnType ecual_button_invoke_on_high(const button_config_t* btn, void (*function_to_invoke)());

#endif /* ECUAL_BUTTON_H */

