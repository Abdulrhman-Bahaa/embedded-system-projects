/**
 ******************************************************************************
 * @file       ecual_char_lcd.c
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains the implementations for char lcd 
 *	       interfaces
 * @date       2023-12-06
 ******************************************************************************
*/
#include "ecual_char_lcd.h"

/* Variables Definitions -----------------------------------------------------*/

/* Functions Implementations -------------------------------------------------*/
#if !defined(CHAR_LCD_I2C)
Std_ReturnType ecual_char_lcd_enable_pulse(const char_lcd_config_t* char_lcd) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        ret |= mcal_gpio_pin_logic_write(&(char_lcd->enable_pin), HIGH);
        _delay_us(2);
        ret |= mcal_gpio_pin_logic_write(&(char_lcd->enable_pin), LOW);
    }
    return ret;
}

/* 4-bit interface  -----------------------------------------------------*/
Std_ReturnType ecual_char_lcd_4_bit_data_send(const char_lcd_config_t* char_lcd, uint8_t to_send_data, logic_t rs_pin) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        ret |= mcal_gpio_pin_logic_write(&(char_lcd->rs_pin), rs_pin);
        for (uint8_t bit_to_pin = 0; bit_to_pin < 4; bit_to_pin++) {
            ret |= mcal_gpio_pin_logic_write(&(char_lcd->data[bit_to_pin]), READ_BIT(to_send_data, bit_to_pin));
        }
        ret |= ecual_char_lcd_enable_pulse(char_lcd);
    }
    return ret;
}

/* 8-bit interface  -----------------------------------------------------*/
Std_ReturnType ecual_char_lcd_8_bit_data_send(const char_lcd_config_t* char_lcd, uint8_t to_send_data, logic_t rs_pin) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        ret |= mcal_gpio_pin_logic_write(&(char_lcd->rs_pin), rs_pin);
        for (uint8_t bit_to_pin = 0; bit_to_pin < 8; bit_to_pin++) {
            ret |= mcal_gpio_pin_logic_write(&(char_lcd->data[bit_to_pin]), READ_BIT(to_send_data, bit_to_pin));
        }
        ret |= ecual_char_lcd_enable_pulse(char_lcd);
    }
    return ret;
}

/* General interface  ---------------------------------------------------*/
Std_ReturnType ecual_char_lcd_data_send(const char_lcd_config_t* char_lcd, uint8_t to_send_data, logic_t rs_pin) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        if (8 == CHAR_LCD_DATA_SIZE) {       
            ret |= ecual_char_lcd_8_bit_data_send(char_lcd, to_send_data, rs_pin);
        }
        else {
            ret |= ecual_char_lcd_4_bit_data_send(char_lcd, to_send_data >> 4, rs_pin);
            ret |= ecual_char_lcd_4_bit_data_send(char_lcd, to_send_data, rs_pin);
        }
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_instruction_send(const char_lcd_config_t* char_lcd, uint8_t instruction ) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        ret |= ecual_char_lcd_data_send(char_lcd, instruction, LOW);
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_init(const char_lcd_config_t* char_lcd, const uint8_t function_set_command, const uint8_t display_control_command) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        for (uint8_t i = 0; i < CHAR_LCD_DATA_SIZE; i++) {
            ret |= mcal_gpio_pin_direction_write(&(char_lcd->data[i]), OUTPUT);
            ret |= mcal_gpio_pin_logic_write(&(char_lcd->data[i]), LOW);
        }
        ret |= mcal_gpio_pin_direction_write(&(char_lcd->rs_pin), OUTPUT);
        ret |= mcal_gpio_pin_logic_write(&(char_lcd->rs_pin), LOW);

        ret |= mcal_gpio_pin_direction_write(&(char_lcd->enable_pin), OUTPUT);
        ret |= mcal_gpio_pin_logic_write(&(char_lcd->enable_pin), LOW);   
        if (4 == CHAR_LCD_DATA_SIZE) {       
            // Function set (Set interface to be 4 bits long.)
            ret |= ecual_char_lcd_instruction_send(char_lcd, function_set_command >> 4);
        }
        else {
            // Function set (Set interface to be 8 bits long.)
            ret |= ecual_char_lcd_instruction_send(char_lcd, function_set_command);
        }
        _delay_us(50);
        // Function set (Interface is 4 bits long. Specify the number of display lines and character font.)
        ret |= ecual_char_lcd_instruction_send(char_lcd, function_set_command);
        _delay_us(50);
        // Display clear
        ret |= ecual_char_lcd_instruction_send(char_lcd, _LCD_CLEAR_DISPLAY);
        _delay_ms(100);
        // Entry mode set
        ret |= ecual_char_lcd_instruction_send(char_lcd, 0x02);
        _delay_us(50);
        // Display on
        ret |= ecual_char_lcd_instruction_send(char_lcd, display_control_command);
        _delay_us(50);
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_cursor_set(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column) {
    Std_ReturnType ret = E_OK;
    if ((NULL == char_lcd) || (row > 4) || (column > 20)) {
        ret = E_NOT_OK;
    }
    else {
        uint8_t cursor_address;
        column--;
        switch(row) {
            case 1:
                cursor_address = 0x0 + column;
                break;
            case 2:
                cursor_address = 0x40 + column;
                break;
            case 3:
                cursor_address = 0x14 + column;
                break;
            case 4:
                cursor_address = 0x54 + column;
                break;
            default:
                ret |= E_NOT_OK;              
        }
        cursor_address += 128;
        ret |= ecual_char_lcd_instruction_send(char_lcd, cursor_address);
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_char_write(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column, uint8_t character) {
     Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        ret = ecual_char_lcd_cursor_set(char_lcd, row, column);
        ret = ecual_char_lcd_data_send(char_lcd, character, HIGH);
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_string_write(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column, char string[]) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        ret = ecual_char_lcd_cursor_set(char_lcd, row, column);
        int i = 0;
        while('\0' != string[i]) {
            ret = ecual_char_lcd_data_send(char_lcd, string[i], HIGH);
            i++;
        }
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_custom_char_add(const char_lcd_config_t* char_lcd, uint8_t custom_char_posn, uint8_t custom_char[]) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd) {
        ret = E_NOT_OK;
    }
    else {
        uint8_t custom_char_address = 0x40 + (custom_char_posn << 3);
        ret |= ecual_char_lcd_instruction_send(char_lcd, custom_char_address);
        for (uint8_t i = 0; i < 8; i++) {
            ret |= ecual_char_lcd_data_send(char_lcd, custom_char[i], HIGH);
        }
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_int_write(const char_lcd_config_t* char_lcd, uint8_t row, uint8_t column, uint32_t num) {
    Std_ReturnType ret = E_OK;     
    if (NULL == char_lcd) {
    ret = E_NOT_OK;
    }
    else {
        ret = ecual_char_lcd_cursor_set(char_lcd, row, column);
        uint8_t arr[10] = {0};
        int8_t i = -1;
        while (num != 0){
            arr[++i] = 48 + (num % 10);
            num /= 10;
        }
        while (i >= 0) {
            ret = ecual_char_lcd_data_send(char_lcd, arr[i], HIGH);
            i--;           
        }
    }
    return ret;
}
#endif

/* I2c interface  --------------------------------------------------------*/
#if defined(CHAR_LCD_I2C)
Std_ReturnType ecual_char_lcd_i2c_data_send(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t to_send_data, logic_t rs_pin) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd_i2c) {
        ret = E_NOT_OK;
    }
    else {
        uint8_t pins_logic = (to_send_data & 0xF0) | ((uint8_t)rs_pin);
        // Enable signal high
        SET_BIT(pins_logic, 2);
        ret |= mcal_mssp_data_trasnmit(pins_logic);
        _delay_us(1);
        // Enable signal low
        CLEAR_BIT(pins_logic, 2);
        ret |= mcal_mssp_data_trasnmit(pins_logic);
        
        pins_logic = (to_send_data << 4)| ((uint8_t)rs_pin);
        // Enable signal high
        SET_BIT(pins_logic, 2);
        ret |= mcal_mssp_data_trasnmit(pins_logic);
        _delay_us(1);
        // Enable signal low
        CLEAR_BIT(pins_logic, 2);
        ret |= mcal_mssp_data_trasnmit(pins_logic);
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_i2c_instruction_send(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t instruction ) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd_i2c) {
        ret = E_NOT_OK;
    }
    else {
        ret |= ecual_char_lcd_i2c_data_send(char_lcd_i2c, instruction, LOW);
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_i2c_init(const char_lcd_i2c_config_t* char_lcd_i2c, const uint8_t function_set_command, const uint8_t display_control_command) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd_i2c) {
        ret = E_NOT_OK;
    }
    else {
        mcal_mssp_start_condition();
        ret |= mcal_mssp_address_trasnmit((char_lcd_i2c->char_lcd_address), WRITE);
        // Function set (Set interface to be 4 bits long.)
        ret |= ecual_char_lcd_i2c_instruction_send(char_lcd_i2c, function_set_command >> 4);
        _delay_us(50);
        // Function set (Interface is 4 bits long. Specify the number of display lines and character font.)
        ret |= ecual_char_lcd_i2c_instruction_send(char_lcd_i2c, function_set_command);
        _delay_us(50);
        // Display clear
        ret |= ecual_char_lcd_i2c_instruction_send(char_lcd_i2c, _LCD_CLEAR_DISPLAY);
        _delay_ms(100);
        // Entry mode set
        ret |= ecual_char_lcd_i2c_instruction_send(char_lcd_i2c, 0x02);
        _delay_us(50);
        // Display on
        ret |= ecual_char_lcd_i2c_instruction_send(char_lcd_i2c, display_control_command);
        _delay_us(50);
        mcal_mssp_stop_condition();
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_i2c_string_write(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t row, uint8_t column, char string[]) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd_i2c) {
        ret = E_NOT_OK;
    }
    else {
        mcal_mssp_start_condition();
        ret |= mcal_mssp_address_trasnmit((char_lcd_i2c->char_lcd_address), WRITE);
        ret |= ecual_char_lcd_i2c_cursor_set(char_lcd_i2c, row, column);
        int i = 0;
        while('\0' != string[i]) {
            ret |= ecual_char_lcd_i2c_data_send(char_lcd_i2c, string[i], HIGH);
            i++;
        }
        mcal_mssp_stop_condition();
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_i2c_cursor_set(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t row, uint8_t column) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd_i2c || row > 4 || column > 20) {
        ret = E_NOT_OK;
    }
    else {
        uint8_t cursor_address;
        column--;
        switch(row) {
            case 1:
                cursor_address = 0x0 + column;
                break;
            case 2:
                cursor_address = 0x40 + column;
                break;
            case 3:
                cursor_address = 0x14 + column;
                break;
            case 4:
                cursor_address = 0x54 + column;
                break;
            default:
                ret = E_NOT_OK;              
        }
        cursor_address += 128;
        ret |= ecual_char_lcd_i2c_instruction_send(char_lcd_i2c, cursor_address);
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_i2c_custom_char_add(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t custom_char_posn, uint8_t custom_char[]) {
    Std_ReturnType ret = E_OK;
    if (NULL == char_lcd_i2c) {
        ret = E_NOT_OK;
    }
    else {
        mcal_mssp_start_condition();
        ret |= mcal_mssp_address_trasnmit((char_lcd_i2c->char_lcd_address), WRITE);
        uint8_t custom_char_address = 0x40 + (custom_char_posn << 3);
        ret |= ecual_char_lcd_i2c_instruction_send(char_lcd_i2c, custom_char_address);
        for (uint8_t i = 0; i < 8; i++) {
            ret |= ecual_char_lcd_i2c_data_send(char_lcd_i2c, custom_char[i], HIGH);
        }
        mcal_mssp_stop_condition();
    }
    return ret;
}

Std_ReturnType ecual_char_lcd_i2c_char_write(const char_lcd_i2c_config_t* char_lcd_i2c, uint8_t row, uint8_t column, uint8_t character) {
     Std_ReturnType ret = E_OK;
    if (NULL == char_lcd_i2c) {
        ret = E_NOT_OK;
    }
    else {
        mcal_mssp_start_condition();
        ret |= mcal_mssp_address_trasnmit((char_lcd_i2c->char_lcd_address), WRITE);
        ret = ecual_char_lcd_i2c_cursor_set(char_lcd_i2c, row, column);
        ret = ecual_char_lcd_i2c_data_send(char_lcd_i2c, character, HIGH);
        mcal_mssp_stop_condition();
    }
    return ret;
}
#endif