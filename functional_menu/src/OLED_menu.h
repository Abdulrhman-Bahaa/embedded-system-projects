 /**
 ******************************************************************************
 * @file       OLED_menu.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for led interfaces
 * @date       2024-5-2
 ******************************************************************************
*/
#ifndef OLED_MENU_H
#define	OLED_MENU_H

/* Includes -----------------------------------------------------------------*/
#include "std/mcal_std_types.h"
#include "oled_display/ecual_oled_display.h"
#include "usart/mcal_usart.h"
#include "pass_auth_system_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "keypad/ecual_keypad.h"

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/
extern const oled_display_config_t oled0;
extern const keypad_config_t keypad0;

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType oled_menu_with_functionality(void);
Std_ReturnType menu_item_change_contrast_fun(uint8_t *keep_in_loop);
Std_ReturnType menu_item_logout_fun(uint8_t *keep_in_loop);
Std_ReturnType menu_item_change_pass_fun(uint8_t *keep_in_loop);
Std_ReturnType menu_item_game_fun(uint8_t *keep_in_loop);

#endif /* OLED_MENU_H */

