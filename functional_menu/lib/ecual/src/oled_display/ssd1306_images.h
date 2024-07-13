/**
 ******************************************************************************
 * @file       ssd1306_images.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations of OLED display module
 * @date       2024-3-4
 ******************************************************************************
*/
#ifndef SSD1306_IMAGES_H
#define	SSD1306_IMAGES_H

/* Includes -----------------------------------------------------------------*/
#include <std/mcal_std_types.h>
#include <avr/pgmspace.h>

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/

/* Variables Declarations ---------------------------------------------------*/
extern const uint8_t assassins[] PROGMEM;
extern const uint8_t blackops[] PROGMEM;
extern const uint8_t tranformers[] PROGMEM;
extern const uint8_t onepiece[] PROGMEM;
extern const uint8_t max[] PROGMEM;
extern const uint8_t batman[] PROGMEM;
extern const uint8_t epd_bitmap_Outline [] PROGMEM;
extern const uint8_t epd_bitmap_lock [] PROGMEM;
extern const uint8_t epd_bitmap_hamad [] PROGMEM;

/* Functions Declarations ---------------------------------------------------*/

#endif	/* SSD1306_IMAGES_H */
