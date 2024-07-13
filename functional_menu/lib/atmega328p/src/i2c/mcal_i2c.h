/**
 ******************************************************************************
 * @file       mcal_i2c.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations of I2C module
 * @date       2024-03-03
 ******************************************************************************
*/
#ifndef MCAL_I2C_H
#define	MCAL_I2C_H

/* Includes -----------------------------------------------------------------*/
#include "../std/mcal_std_types.h"
#include "../gpio/mcal_gpio.h"
#include "../interrupts/internal/mcal_internal_interrupts.h"

/* Macro Declarations -------------------------------------------------------*/
#define I2C_ENABLE()    (SET_BIT(TWCR, TWEN))
#define I2C_DISABLE()   (CLEAR_BIT(TWCR, TWEN))

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
typedef enum {
    WRITE = 0,
    READ
}i2c_master_operation_t;

typedef struct {
    uint32_t serial_clock;
}i2c_config_t;

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType mcal_i2c_init(void);
void mcal_i2c_start_condition(void);
void mcal_i2c_stop_condition(void);
Std_ReturnType mcal_i2c_address_trasnmit(const uint8_t slave_address, i2c_master_operation_t master_operation);
Std_ReturnType mcal_i2c_data_trasnmit(const uint8_t data_to_transmit);

#endif	/* MCAL_I2C_H */
