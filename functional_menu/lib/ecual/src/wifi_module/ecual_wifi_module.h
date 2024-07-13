 /**
 ******************************************************************************
 * @file       ecual_wifi_module.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the declarations for wifi module
 *             interfaces
 * @date       2024-3-08
 ******************************************************************************
*/
#ifndef ECUAL_WIFI_MODULE_H
#define	ECUAL_WIFI_MODULE_H

/* Includes -----------------------------------------------------------------*/
#include <usart/mcal_usart.h>
#include <stdio.h>
#include "string.h"


/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
typedef struct {
    pin_config_t en_pin;
    pin_config_t rst_pin;
}wifi_module_config_t;

typedef struct{
    uint8_t* key;
    uint16_t value;
}key_value_pair_t;

/* Functions Declarations ---------------------------------------------------*/
Std_ReturnType ecual_wifi_module_wifi_connect(const char* wifi_ssid, const char* wifi_pwd);
Std_ReturnType ecual_wifi_module_wifi_data_send(void);
Std_ReturnType ecual_wifi_module_http_request(const char* method, const char* host, const char* target_path);
Std_ReturnType ecual_wifi_module_ubidots_data_send(const char* token, const char* device_label, const key_value_pair_t array[], uint8_t start_index, uint8_t end_index);
Std_ReturnType ecual_wifi_module_ubidots_variable_send(const char* token, const char* device_label, const char* var_label, const char* var_value);
#endif /* ECUAL_WIFI_MODULE_H */

