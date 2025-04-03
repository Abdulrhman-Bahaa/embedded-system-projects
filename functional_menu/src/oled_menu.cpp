 /**
 ******************************************************************************
 * @file       oled_menu.c
 * @author     Abdulrhman Bahaa
 * @brief      This header file contains the implementations for keypad interfaces
 * @date       2024-5-2
 ******************************************************************************
*/
#include "oled_menu.h"

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
/* Variables Definitions -----------------------------------------------------*/
uint8_t* menu_items[] = {"Unimplemented", "Unimplemented", "Unimplemented", "Unimplemented", "Game", "Contrast", "Change pass", "Log out"};

const oled_display_menu_config_t menu0 = {
	.oled_display = &oled0,
	.number_of_items = 8,
	.font_width = 6,
	.font_height = 8,
	.array_of_items = menu_items
};

Std_ReturnType (*menu_items_functions[])(uint8_t *keep_in_loop) = 
        {NULL, NULL, NULL, NULL, &menu_item_game_fun, &menu_item_change_contrast_fun, &menu_item_change_pass_fun, &menu_item_logout_fun};

/* Functions Implementations ------------------------------------------------*/
Std_ReturnType oled_menu_with_functionality(void) {
    Std_ReturnType ret = E_OK;
    uint8_t received_char = 0, selected_item = 0, keep_in_loop = 1, items_num = menu0.number_of_items;
    ret |= ecual_oled_display_menu_init(&menu0, 0);  
    while(keep_in_loop) {
        ret |= mcal_usart_receive_data(&received_char);
        if (received_char) {
            switch (received_char) {
            case 'w':   // Go up
                selected_item--;
                selected_item = (selected_item + items_num) % items_num;
                ret = ecual_oled_display_menu_item_select(&menu0, selected_item);
                break;
            case 'e':   // Select
                menu_items_functions[selected_item](&keep_in_loop);
                selected_item = 0;
                break;
            case 's':   // Go down
                selected_item++;
                selected_item = (selected_item + items_num) % items_num;
                ret = ecual_oled_display_menu_item_select(&menu0, selected_item);
                break;
            default:
                ret = E_NOT_OK;
                break;
            }
            received_char = 0;
        }
    }
    return ret;
}

Std_ReturnType menu_item_game_fun(uint8_t *keep_in_loop) {
    Std_ReturnType ret = E_OK;
    uint8_t received_char = 0, down_targets_str[3], keep_in_curr_loop = 1, targets_offset = 10,
    target_width = 10, target_horizontal_pos = 0, target_vertical_pos = 8, down_targets = 0, random_num = 0, player_width = 36,
    step_size = 5, player_horizontal_pos = ((128 - player_width) / 2), player_vertical_pos = 7, bullet_horizontal_pos = player_horizontal_pos + (player_width / 2),
    target_down = 0;
    uint16_t target_time = 58000,  k = 0;
    srand(time(0));
    ret |= ecual_oled_display_clear(&oled0);

    // The player
    ret |= ecual_oled_display_drawer(&oled0, 0xff, player_vertical_pos, player_horizontal_pos, player_vertical_pos,  player_horizontal_pos + player_width);
    while(keep_in_curr_loop) {

        // Generate targets each period of time
        if (target_vertical_pos == 8 || (1 == target_down) ) {
            _delay_ms(100);
            ret |= ecual_oled_display_drawer(&oled0, 0x00, target_vertical_pos - 1, target_horizontal_pos, target_vertical_pos - 1, target_horizontal_pos + target_width);
            random_num = rand();
            target_horizontal_pos = targets_offset + (random_num % 80);
            target_vertical_pos = 1;
            target_down = 0;
        }
        else if (target_time == k) {
            ret |= ecual_oled_display_drawer(&oled0, 0xff, target_vertical_pos, target_horizontal_pos, target_vertical_pos, target_horizontal_pos + target_width);
            ret |= ecual_oled_display_drawer(&oled0, 0x00, target_vertical_pos - 1, target_horizontal_pos, target_vertical_pos - 1, target_horizontal_pos + target_width);
            target_vertical_pos++;
            k = 0;
        }
        else  {
            k++;
        }
        if ((((player_horizontal_pos <= target_horizontal_pos) && (player_horizontal_pos + player_width >= target_horizontal_pos + target_width)) ||
            ((player_horizontal_pos < target_horizontal_pos) && (player_horizontal_pos + player_width < target_horizontal_pos + target_width) && (player_horizontal_pos + player_width > target_horizontal_pos)) ||
            ((player_horizontal_pos > target_horizontal_pos) && (player_horizontal_pos + player_width > target_horizontal_pos + target_width) && (player_horizontal_pos < target_horizontal_pos + target_width))
            )
             && (7 == target_vertical_pos)) {
            ret |= ecual_oled_display_string_write(&oled0, "Game over", 8, 16, 3, 30);
            keep_in_curr_loop = 0;
        }
        // ret |= ecual_keypad_char_read(&keypad0, &received_char);
        ret |= mcal_usart_receive_data(&received_char);
		if (received_char) {
			switch (received_char) {
			case 'd':   // Go right
                player_horizontal_pos += step_size;
                bullet_horizontal_pos += step_size;
                // Erase the previous step
                ret |= ecual_oled_display_drawer(&oled0, 0x00, player_vertical_pos, player_horizontal_pos - step_size, player_vertical_pos, player_horizontal_pos);
				break;
			case '*':    // Exit
                keep_in_curr_loop = 0;
                break;
			case 'a':   // Go left
                player_horizontal_pos -= step_size;
                bullet_horizontal_pos -= step_size;
                // Erase the previous step
                ret |= ecual_oled_display_drawer(&oled0, 0x00, player_vertical_pos, player_horizontal_pos + player_width, player_vertical_pos, player_horizontal_pos + step_size + player_width);
				break;
            case ' ':   // Shoot
                uint8_t j = 0;
                if ((bullet_horizontal_pos >= target_horizontal_pos) && (bullet_horizontal_pos <= target_horizontal_pos + target_width)) {
                    down_targets++;
                    sprintf(down_targets_str, "%d", down_targets);
                    ret |= ecual_oled_display_string_write(&oled0, down_targets_str, 6, 8, 0, 0);
                    target_down = 1;
                    target_time-=1000;
                }
                else {
                    ret |= ecual_oled_display_string_write(&oled0, "Game over", 8, 16, 3, 30);
                    keep_in_curr_loop = 0;
                }
                for (sint8_t i = 6; i > 0; i--) {
                    // Draw the current bullet
                    ret |= ecual_oled_display_drawer(&oled0, 0x18, i, bullet_horizontal_pos - 2, i, bullet_horizontal_pos + 2);
                    ret |= ecual_oled_display_drawer(&oled0, 0x3C, i, bullet_horizontal_pos - 1, i, bullet_horizontal_pos + 1);
                    // Erase the previous bullet
                    ret |= ecual_oled_display_drawer(&oled0, 0x00, j, bullet_horizontal_pos - 2, j, bullet_horizontal_pos + 2);
                    ret |= ecual_oled_display_drawer(&oled0, 0x00, j, bullet_horizontal_pos - 1, j, bullet_horizontal_pos + 1);
                    j = i;
                    _delay_ms(50);
                }
                // Erase the previous bullet
                ret |= ecual_oled_display_drawer(&oled0, 0x00, j, bullet_horizontal_pos - 2 , j, bullet_horizontal_pos + 2);
				break;
			default:
                ret = E_NOT_OK;
				break;
			}
            ret |= ecual_oled_display_drawer(&oled0, 0xff, player_vertical_pos, player_horizontal_pos, player_vertical_pos, player_horizontal_pos + player_width);
			received_char = 0;
		}
		else {
			/* Nothing */
		}
    }
    _delay_ms(2000);
    ret |= ecual_oled_display_menu_init(&menu0, 0);
    return ret;
}

Std_ReturnType menu_item_change_contrast_fun(uint8_t *keep_in_loop) {
    Std_ReturnType ret = E_OK;
    uint8_t i = 0, received_char = 0, bar_width_str[3], keep_in_curr_loop = 1;
    static sint8_t bar_width = 25;
    sprintf(bar_width_str, "%i", bar_width);
    ret |= ecual_oled_display_clear(&oled0);
    ret |= ecual_oled_display_rectangle_draw(&oled0, 1, 2, 38, 52, 16);
    ret |= ecual_oled_display_drawer(&oled0, 0xff, 2, 38, 2, 38 + bar_width);
    ret |= ecual_oled_display_string_write(&oled0, bar_width_str, 8, 16, 4, 55);
    while(keep_in_curr_loop) {
        ret |= mcal_usart_receive_data(&received_char);
		if (received_char) {
			switch (received_char) {
			case 'd':
                bar_width += 5;
				break;
			case 'e':
                keep_in_curr_loop = 0;
                break;
			case 'a':
                bar_width -= 5;
				break;
			default:
				break;
			}
            if (bar_width > 50) {
                bar_width = 50;
            }
            else if (bar_width < 5) {
                bar_width = 5;
            }
            else {

            }
			received_char = 0;
            ret |= ecual_oled_display_drawer(&oled0, 0x00, 2, 38, 2, 38 + 51);
            ret |= ecual_oled_display_drawer(&oled0, 0xff, 2, 38, 2, 38 + bar_width);
            sprintf(bar_width_str, "%i", bar_width);
            ret |= ecual_oled_display_string_write(&oled0, bar_width_str, 8, 16, 4, 55);
            ret |= ecual_oled_display_contrast_set(&oled0, (5 * bar_width));
		}
		else {
			/* Nothing */
		}
    }
    return ret;
}

Std_ReturnType menu_item_logout_fun(uint8_t *keep_in_loop) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_oled_display_clear(menu0.oled_display);
    ret |= ecual_oled_display_bit_mapping(&oled0, epd_bitmap_lock, 128, 64, 0, 0);
    *keep_in_loop = 0;
    return ret;
}

Std_ReturnType menu_item_change_pass_fun(uint8_t *keep_in_loop) {
    Std_ReturnType ret = E_OK;
    ret |= ecual_oled_display_clear(menu0.oled_display);
    ret |= ecual_oled_display_string_write(&oled0, "Enter new pass", 8, 16, 0, 0);
    ret |= system_config();
    ret |= ecual_oled_display_menu_init(&menu0, 0);
    return ret;
}


