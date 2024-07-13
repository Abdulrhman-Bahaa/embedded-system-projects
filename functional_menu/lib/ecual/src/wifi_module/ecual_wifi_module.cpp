 /**
 ******************************************************************************
 * @file       ecual_wifi_module.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for wifi module
 *             interfaces
 * @date       2024-3-08
 ******************************************************************************
*/
#include "ecual_wifi_module.h"

/* Global Variables ---------------------------------------------------------*/

/* Functions Implementations ------------------------------------------------*/
void array_to_dictionary(const key_value_pair_t arr[], uint8_t start_index, uint8_t end_index, uint8_t *dictionary) {
    strcpy(dictionary, "{");
    for (uint8_t i = start_index; i < end_index; i++) {
        strcat(dictionary, "\"");
        strcat(dictionary, arr[i].key);
        strcat(dictionary, "\": ");
        uint8_t valueStr[5];
        sprintf(valueStr, "%d", arr[i].value);
        strcat(dictionary, valueStr);
        if (i < (end_index - start_index - 1)) {
            strcat(dictionary, ", ");
        }
    }
    strcat(dictionary, "}");
}

Std_ReturnType ecual_wifi_module_wifi_connect(const char* wifi_ssid, const char* wifi_pwd) {
    Std_ReturnType ret = E_OK;
    // Station mode
    ret |= mcal_usart_transmit_string("AT+CWMODE=1\r\n");
    _delay_ms(100);
    uint8_t command[60] =  "AT+CWJAP=\"";
    uint8_t i = 0;
    strcat(command, wifi_ssid);
    strcat(command, "\",\"");
    strcat(command, wifi_pwd);
    strcat(command, "\"\r\n");
    ret |= mcal_usart_transmit_string(command);
    return ret;
}

Std_ReturnType ecual_wifi_module_http_request(const char* method, const char* host, const char* target_path) {
    Std_ReturnType ret = E_OK;
     uint8_t tcp_connection[60] =  "AT+CIPSTART=\"TCP\",\"";
     uint8_t data_length_str[5];
     uint8_t data_length = 41;
     strcat(tcp_connection, host);
     strcat(tcp_connection, "\",80\r\n");
    data_length += strlen(method);
    data_length += strlen(target_path);
    data_length += strlen(host);
    ret |= mcal_usart_transmit_string(tcp_connection);
    _delay_ms(1000);
    sprintf(data_length_str, "%d", data_length);
    ret |= mcal_usart_transmit_string("AT+CIPSEND=");
    ret |= mcal_usart_transmit_string(data_length_str);
    ret |= mcal_usart_transmit_string("\r\n");
    _delay_ms(300);
    ret |= mcal_usart_transmit_string(method);
    ret != mcal_usart_transmit_data(' ');
    ret |= mcal_usart_transmit_string(target_path);
    ret |= mcal_usart_transmit_string(" HTTP/1.1\r\n");
    ret |= mcal_usart_transmit_string("Host: ");
    ret |= mcal_usart_transmit_string(host);
    ret |= mcal_usart_transmit_string("\r\n");
    ret |= mcal_usart_transmit_string("Connection: close\r\n\r\n");
    return ret;
}

Std_ReturnType ecual_wifi_module_ubidots_data_send(const char* token, const char* device_label, const key_value_pair_t array[], uint8_t start_index, uint8_t end_index) {
    Std_ReturnType ret = E_OK;
    uint8_t tcp_connection[60] =  "AT+CIPSTART=\"TCP\",\"", json_string[50], data_length_str[5], content_length_str[3], data_length = 139, content_length = 0,
    host[] = "industrial.api.ubidots.com", target_path[30] = "/api/v1.6/devices/";

    array_to_dictionary(array, start_index, end_index, json_string);

    // Init tcp connection
    strcat(tcp_connection, host);
    strcat(tcp_connection, "\",80\r\n");
    ret |= mcal_usart_transmit_string(tcp_connection);
    _delay_ms(1000);

    // Concat strings and calc size in bytes
    strcat(target_path, device_label);
    strcat(target_path, "/");
    data_length += strlen(target_path);
    data_length += strlen(host);
    data_length += strlen(token);
    data_length += strlen(json_string);
    content_length = strlen(json_string);
    sprintf(data_length_str, "%d", data_length);
    sprintf(content_length_str, "%d", content_length);

    // Start data transmit
    ret |= mcal_usart_transmit_string("AT+CIPSEND=");
    ret |= mcal_usart_transmit_string(data_length_str);
    ret |= mcal_usart_transmit_string("\r\n");
    _delay_ms(300);

    ret |= mcal_usart_transmit_string("POST ");
    ret |= mcal_usart_transmit_string(target_path);
    ret |= mcal_usart_transmit_string(" HTTP/1.1\r\n");

    ret |= mcal_usart_transmit_string("Host: ");
    ret |= mcal_usart_transmit_string(host);
    ret |= mcal_usart_transmit_string("\r\n");

    ret |= mcal_usart_transmit_string("User-Agent: ESP8266/1.0\r\n");

    ret |= mcal_usart_transmit_string("X-Auth-Token: ");
    ret |= mcal_usart_transmit_string(token);
    ret |= mcal_usart_transmit_string("\r\n");

    ret |= mcal_usart_transmit_string("Content-Type: application/json\r\n");

    ret |= mcal_usart_transmit_string("Connection: close\r\n");

    ret |= mcal_usart_transmit_string("Content-Length: ");
    ret |= mcal_usart_transmit_string(content_length_str);
    ret |= mcal_usart_transmit_string("\r\n\r\n");

    ret |= mcal_usart_transmit_string(json_string);
    ret |= mcal_usart_transmit_string("\r\n");
    return ret;
}

Std_ReturnType ecual_wifi_module_ubidots_variable_send(const char* token, const char* device_label, const char* var_label, const char* var_value) {
    Std_ReturnType ret = E_OK;
    uint8_t tcp_connection[60] =  "AT+CIPSTART=\"TCP\",\"", json_string[30] = {"{\""}, data_length_str[5], var_value_str[10], content_length_str[3], data_length = 139, content_length = 0,
    host[] = "industrial.api.ubidots.com", target_path[30] = "/api/v1.6/devices/";

    strcat(json_string, var_label);
    strcat(json_string, "\": ");
    strcat(json_string, var_value);
    strcat(json_string, "}");

    // Init tcp connection
    strcat(tcp_connection, host);
    strcat(tcp_connection, "\",80\r\n");
    ret |= mcal_usart_transmit_string(tcp_connection);
    _delay_ms(1000);

    // Concat strings and calc size in bytes
    strcat(target_path, device_label);
    strcat(target_path, "/");
    data_length += strlen(target_path);
    data_length += strlen(host);
    data_length += strlen(token);
    data_length += strlen(json_string);
    content_length = strlen(json_string);
    sprintf(data_length_str, "%d", data_length);
    sprintf(content_length_str, "%d", content_length);

    // Start data transmit
    ret |= mcal_usart_transmit_string("AT+CIPSEND=");
    ret |= mcal_usart_transmit_string(data_length_str);
    ret |= mcal_usart_transmit_string("\r\n");
    _delay_ms(300);

    ret |= mcal_usart_transmit_string("POST ");
    ret |= mcal_usart_transmit_string(target_path);
    ret |= mcal_usart_transmit_string(" HTTP/1.1\r\n");

    ret |= mcal_usart_transmit_string("Host: ");
    ret |= mcal_usart_transmit_string(host);
    ret |= mcal_usart_transmit_string("\r\n");

    ret |= mcal_usart_transmit_string("User-Agent: ESP8266/1.0\r\n");

    ret |= mcal_usart_transmit_string("X-Auth-Token: ");
    ret |= mcal_usart_transmit_string(token);
    ret |= mcal_usart_transmit_string("\r\n");

    ret |= mcal_usart_transmit_string("Content-Type: application/json\r\n");

    ret |= mcal_usart_transmit_string("Connection: close\r\n");

    ret |= mcal_usart_transmit_string("Content-Length: ");
    ret |= mcal_usart_transmit_string(content_length_str);
    ret |= mcal_usart_transmit_string("\r\n\r\n");

    ret |= mcal_usart_transmit_string(json_string);
    ret |= mcal_usart_transmit_string("\r\n");
    return ret;
}


