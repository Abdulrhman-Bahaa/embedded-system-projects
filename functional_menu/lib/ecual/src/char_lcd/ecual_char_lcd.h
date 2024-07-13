 /**
 ******************************************************************************
 * @file       ecual_char_lcd.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for char lcd 
 *             interfaces
 * @date       2023-12-06
 ******************************************************************************
*/
#ifndef ECUAL_CHAR_LCD_H
#define	ECUAL_CHAR_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "ecual_char_lcd_configs.h"
#if !defined(CHAR_LCD_I2C)
#include <atmega328.h>
#endif
#if defined(CHAR_LCD_I2C)
#include "i2c/mcal_i2c.h"
#endif

/* Macro Declarations --------------------------------------------------------*/
#define LCD_FIRST_ROW                       0x01
#define LCD_SECOND_ROW                      0x02

#define _LCD_8_BIT_2_LINES_FONT_5X8_DOTS    0x38
#define _LCD_4_BIT_2_LINES_FONT_5X8_DOTS    0x28
#define _LCD_CLEAR_DISPLAY                  0x01
#define _LCD_RETURN_HOME                    0x02
#define _LCD_DISPLAY_OFF_CURSOR_OFF         0x08
#define _LCD_DISPLAY_ON_CURSOR_OFF          0x0C
#define _LCD_DISPLAY_ON_CURSOR_ON           0x0E

/* Macro Functions Declarations ----------------------------------------------*/

/* Data Type Declarations ----------------------------------------------------*/
#if !defined(CHAR_LCD_I2C)
typedef struct {
    pin_location_t rs_pin;
    pin_location_t enable_pin;
    pin_location_t data[CHAR_LCD_DATA_SIZE];
}char_lcd_config_t;
#endif

#if defined(CHAR_LCD_I2C)
typedef struct {
    uint8_t char_lcd_address;
}char_lcd_i2c_config_t;
#endif

/* Variables Declarations ----------------------------------------------------*/

/* Functions Declarations ----------------------------------------------------*/
#if !defined(CHAR_LCD_I2C)
Std_ReturnType ecual_char_lcd_enable_pulse(const char_lcd_config_t* char_lcd);
Std_ReturnType ecual_char_lcd_4_bit_data_send(const char_lcd_config_t* char_lcd, uint8_t to_send_data, logic_t rs_pin);
Std_ReturnType ecual_char_lcd_8_bit_data_send(const char_lcd_config_t* char_lcd, uint8_t to_send_data, logic_t rs_pin);
Std_ReturnType ecual_char_lcd_data_send(const char_lcd_config_t* char_lcd, uint8_t to_send_data, logic_t rs_pin);
Std_ReturnType ecual_char_lcd_instruction_send(const char_lcd_config_t* char_lcd, uint8_t instruction );
Std_ReturnType ecual_char_lcd_init(const char_lcd_config_t* char_lcd, const uint8_t function_set_command, const uint8_t display_control_command);
Std_ReturnType ecual_char_lcd_cursor_set(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column);
Std_ReturnType ecual_char_lcd_custom_char_add(const char_lcd_config_t* char_lcd, uint8_t custom_char_posn, uint8_t custom_char[]);
Std_ReturnType ecual_char_lcd_char_write(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column, uint8_t character);
Std_ReturnType ecual_char_lcd_string_write(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column, char string[]);
Std_ReturnType ecual_char_lcd_int_write(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column, uint32_t num);
#endif

/* I2c interface  --------------------------------------------------------*/
#if defined(CHAR_LCD_I2C)
Std_ReturnType ecual_char_lcd_i2c_data_send(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t to_send_data, logic_t rs_pin);
Std_ReturnType ecual_char_lcd_i2c_init(const char_lcd_i2c_config_t* char_lcd_i2c, const uint8_t function_set_command, const uint8_t display_control_command);
Std_ReturnType ecual_char_lcd_i2c_cursor_set(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t row, uint8_t column);
Std_ReturnType ecual_char_lcd_i2c_custom_char_add(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t custom_char_posn, uint8_t custom_char[]);
Std_ReturnType ecual_char_lcd_i2c_char_write(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t row, uint8_t column, uint8_t character);
Std_ReturnType ecual_char_lcd_i2c_string_write(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t row, uint8_t column, char string[]);
#endif

#endif	/* ecual_CHAR_LCD_H */
