 /**
 ******************************************************************************
 * @file       ecual_led.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for keypad interfaces
 * @date       2024-3-15
 ******************************************************************************
*/
#include "ecual_keypad.h"

/* Variables Definitions -----------------------------------------------------*/
static const uint8_t keys[KEYPAD_ROWS_NUMBER][KEYPAD_COLUMNS_NUMBER] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

/* Functions Implementations ------------------------------------------------*/
Std_ReturnType ecual_keypad_init(const keypad_config_t* keypad) {
    Std_ReturnType ret = E_OK;
    if(NULL == keypad) {
        ret = E_NOT_OK;
    }
    else {
        for (uint8_t column_num = 0; column_num < KEYPAD_COLUMNS_NUMBER; column_num++) {
            ret |= mcal_gpio_pin_direction_write(&(keypad->columns[column_num]), OUTPUT); 
            ret |= mcal_gpio_pin_logic_write(&(keypad->columns[column_num]), LOW);
        }
        for (uint8_t row_num = 0; row_num < KEYPAD_ROWS_NUMBER; row_num++) {
            ret |= mcal_gpio_pin_direction_write(&(keypad->rows[row_num]), INPUT); 
            ret |= mcal_gpio_pin_logic_write(&(keypad->rows[row_num]), HIGH);
        }
    }
    return ret;
}

Std_ReturnType ecual_keypad_char_read(const keypad_config_t* keypad, uint8_t* received_char) {
    Std_ReturnType ret = E_OK;
    if (NULL == keypad) {
        ret = E_NOT_OK;
    }
    else {
        logic_t pin_logic = LOW;
        uint8_t columns_logic = 0x00;

        for (uint8_t column_num = 0; column_num < KEYPAD_COLUMNS_NUMBER; column_num++) {
            columns_logic = 0xff;
            CLEAR_BIT(columns_logic, column_num);
            ret |= mcal_gpio_pin_logic_write(&(keypad->columns[0]), READ_BIT(columns_logic, 0));
            ret |= mcal_gpio_pin_logic_write(&(keypad->columns[1]), READ_BIT(columns_logic, 1));
            ret |= mcal_gpio_pin_logic_write(&(keypad->columns[2]), READ_BIT(columns_logic, 2));
            for (uint8_t row_num = 0; row_num < KEYPAD_ROWS_NUMBER; row_num++) {  
                ret |= mcal_gpio_pin_logic_read(&(keypad->rows[row_num]), &pin_logic);
                if (LOW == pin_logic) {
                    *received_char = keys[row_num][column_num];      
                    column_num = KEYPAD_COLUMNS_NUMBER;
                    row_num = KEYPAD_ROWS_NUMBER;
                }
                else {
                    /* Nothing */
                }   
            }
        }
        _delay_ms(50);
    }
    return ret;
}

