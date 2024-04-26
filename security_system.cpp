/**
 ******************************************************************************
 * @mainpage   Security System
 * @brief      This source file contains the main function of the application
 * @date       2024-3-27
 ******************************************************************************
*/
#include <ecual.h>

/* Variables Definitions ----------------------------------------------------*/ 
uint8_t password[] = "00000", password_length = 5, received_password[6] = {'\0'}, token[] = "";
sint8_t access_attempts = 3;
key_value_pair_t arr[] = {{"access log", 0}, {"limit exceeded", 0}};

const rgb_led_config_t rgb0 = {
    .red_location = {
        .port = PORTD_INDEX,
        .pin = PIN3_INDEX
    },
    .blue_location = {
        .port = PORTD_INDEX,
        .pin = PIN5_INDEX
    },
    .green_location = {
        .port = PORTD_INDEX,
        .pin = PIN4_INDEX
    }
};

const pin_config_t buzzer0 = {
    .location = {
        .port = PORTB_INDEX,
        .pin = PIN5_INDEX
    },
    .direction = OUTPUT,
    .logic = LOW
};

const keypad_config_t keypad0 = {
    .columns = {
        {PORTB_INDEX, PIN2_INDEX},
        {PORTB_INDEX, PIN3_INDEX},
        {PORTB_INDEX, PIN4_INDEX}
    },
    .rows = {
        {PORTD_INDEX, PIN6_INDEX},
        {PORTD_INDEX, PIN7_INDEX},
        {PORTB_INDEX, PIN0_INDEX},
        {PORTB_INDEX, PIN1_INDEX}
    }
};

const oled_display_config_t oled0 = {
    .oled_display_address = 0x78
};
            
usart_config_t usart0 = {
    .mode = USART_ASYNCHRONOUS_MODE,
    .ubrr_value = 16,
    .device_state = USART_TRANSMITTER_RECEIVER,
    .usart_interrupt = 0,
    .usart_receive_callback_interrupt_function = NULL
};

/* Initialization Function --------------------------------------------------*/ 
Std_ReturnType application_initialize(void) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_rgb_led_init(&rgb0);
    ret |= ecual_oled_display_init(&oled0);
    ret |= ecual_keypad_init(&keypad0);
    ret |= mcal_gpio_pin_init(&buzzer0);    
    ret |= mcal_usart_init(&usart0);
    return ret;
}

/* Main Function ------------------------------------------------------------*/
int main(void) {
    Std_ReturnType ret = E_OK;
    ret |= application_initialize();
    uint8_t recieved_char = 0, start_column = 36, clear = 1;

    ret |= ecual_oled_display_bit_mapping(&oled0, epd_bitmap_lock, 128, 64, 0, 0);    
    // Do foreever
    while(1) {
        // If there are allowed attempts
        if (access_attempts > 0) { 
            ret |= ecual_keypad_char_read(&keypad0, &recieved_char);
            // If the required number (5) of the password digits received
            if (0 == password_length) {
                ret |= ecual_oled_display_clear(&oled0);
                // If received password is correct : green led, print "Access granted", make sound.
                if (!compare_strings(received_password, password)) {
                    ret |= ecual_rgb_led_turn_on(&rgb0, RGP_LED_GREEN);
                    ret |= ecual_oled_display_string_write(&oled0, "Access granted  Welcome", 8, 16, 2, 10);
                    // Buzzer sound
                    ret |= mcal_gpio_pin_logic_write(&(buzzer0.location), HIGH);
                    _delay_ms(100);
                    ret |= mcal_gpio_pin_logic_write(&(buzzer0.location), LOW);
		    _delay_ms(500);
                    ret |= ecual_rgb_led_turn_off(&rgb0);
                    arr[0].value = 0;
                }
                // If received password is incorrect : red led, print "Access denied", make sound.
                else {
                    ret |= ecual_rgb_led_turn_on(&rgb0, RGP_LED_RED);  
                    ret |= ecual_oled_display_string_write(&oled0, "Access denied", 8, 16, 0, 13);
                    // Buzzer sound
                    ret |= mcal_gpio_pin_logic_write(&(buzzer0.location), HIGH);
                    _delay_ms(400);
                    ret |= mcal_gpio_pin_logic_write(&(buzzer0.location), LOW);
                    _delay_ms(400);
                    ret |= mcal_gpio_pin_logic_write(&(buzzer0.location), HIGH);
                    _delay_ms(400);
                    ret |= mcal_gpio_pin_logic_write(&(buzzer0.location), LOW);
                    ret |= ecual_rgb_led_turn_off(&rgb0);
                    access_attempts--;
                    arr[0].value = 1;
                }
                ret |= ecual_wifi_module_ubidots_data_send(token, "esp8266", arr, 0, 1);
                password_length = 5;
                clear = 1;
                start_column = 36;
            }
            else {
                if (recieved_char) {
                    if (1 == clear) {
                        ret |= ecual_oled_display_clear(&oled0);
                        ret |= ecual_rgb_led_turn_off(&rgb0);
                        clear = 0;
                    }
                    else {
                        /* Nothing */
                    }
                    ret |= ecual_oled_display_char_write(&oled0, recieved_char, 11, 16,  2, start_column);
                    received_password[5 - password_length] = recieved_char;
                    start_column += 11;
                    password_length--;
                    recieved_char = 0;
                }
                else {
                    /* Nothing */
                }
            }
        }
        else if (0 == access_attempts) {
            ret |= ecual_oled_display_string_write(&oled0, "No more attemptsallowed", 8, 16, 0, 0);
            _delay_ms(4000);
            ret |= ecual_oled_display_bit_mapping(&oled0, epd_bitmap_lock, 128, 64, 0, 0);
            access_attempts--;
            arr[1].value = 1;
            ret |= ecual_wifi_module_ubidots_data_send(token, "esp8266", arr, 1, 2); 
	}
        else {
		/* Nothing */
        }
    }
    return 0;
}
    
/* Functions Implementations -------------------------------------------------*/
