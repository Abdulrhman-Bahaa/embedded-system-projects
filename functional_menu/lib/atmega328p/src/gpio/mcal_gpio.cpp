/**
 ******************************************************************************
 * @file       mcal_gpio.c
 * @author     Abdulrhman Bahaa
 * @brief      This source file contains implementations for the gpio module
 *             functions
 * @date       2023-12-06
 ******************************************************************************
*/
#include "mcal_gpio.h"

/* Variables Definitions ----------------------------------------------------*/
volatile uint8_t* const ddr_register[PORT_MAX_NUMBER] = {&DDRB ,&DDRC, &DDRD};
volatile uint8_t* const port_register[PORT_MAX_NUMBER] = {&PORTB, &PORTC, &PORTD};
volatile uint8_t* const pin_register[PORT_MAX_NUMBER] = {&PINB, &PINC, &PIND};

/* Functions Implementations -------------------------------------------------*/
Std_ReturnType mcal_gpio_pin_direction_write(const pin_location_t* pin_location, const direction_t direction) {
    Std_ReturnType ret = E_OK;
    uint8_t bit_posn = pin_location->pin;
    if((NULL == pin_location) || (bit_posn > PORT_PIN_MAX_NUMBER - 1)) {
        ret = E_NOT_OK;
    } 
    else {
        volatile uint8_t* reg_addr = ddr_register[pin_location->port];
        switch(direction){
            case INPUT:
                CLEAR_BIT(*reg_addr,bit_posn);
                break;
            case OUTPUT:
                SET_BIT(*reg_addr,bit_posn);
                break;
            default: 
                ret = E_NOT_OK;
        }
    }
    return ret;  
}

Std_ReturnType mcal_gpio_pin_logic_write(const pin_location_t* pin_location, const logic_t logic) {
    Std_ReturnType ret = E_OK;
    uint8_t bit_posn = pin_location->pin;
    if((NULL == pin_location) || (bit_posn > PORT_PIN_MAX_NUMBER - 1)) {
        ret = E_NOT_OK;
    } 
    else {
        volatile uint8_t* reg_addr = port_register[pin_location->port];
        switch(logic) {
            case HIGH:
                SET_BIT(*reg_addr,bit_posn);
                break;
            case LOW:
                CLEAR_BIT(*reg_addr,bit_posn);
                break;
            default:
                ret = E_NOT_OK;
        }
    }
    return ret;  
}

Std_ReturnType mcal_gpio_pin_logic_read(const pin_location_t* pin_location, logic_t* logic) {
    Std_ReturnType ret = E_OK;
    uint8_t bit_posn = pin_location->pin;
    if ((NULL == pin_location) || (bit_posn > PORT_PIN_MAX_NUMBER - 1)) {
        ret = E_NOT_OK;
    }
    else {
        volatile uint8_t* reg_addr = pin_register[pin_location->port];
        *logic = READ_BIT(*reg_addr,bit_posn); 
    }
    return ret;  
}

Std_ReturnType mcal_gpio_pin_init(const pin_config_t* pin_config){
    Std_ReturnType ret = E_OK;
    if(NULL == pin_config) {
        ret = E_NOT_OK;
    }
    else {
        ret |= mcal_gpio_pin_direction_write(&(pin_config->location), pin_config->direction);
        ret |= mcal_gpio_pin_logic_write(&(pin_config->location), pin_config->logic);
    }
    return ret;  
}

Std_ReturnType mcal_gpio_pin_logic_toggle(const pin_location_t* pin_location) {
    Std_ReturnType ret = E_OK;
    uint8_t bit_posn = pin_location->pin;
    if((NULL == pin_location) || (bit_posn > PORT_PIN_MAX_NUMBER - 1)) {
        ret = E_NOT_OK;
    }
    else {
        volatile uint8_t* reg_addr = port_register[pin_location->port];
        TOGGLE_BIT(*reg_addr, bit_posn);
    } 
    return ret;  
}
