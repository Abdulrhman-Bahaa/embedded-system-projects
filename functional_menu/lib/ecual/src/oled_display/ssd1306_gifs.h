/**
 ******************************************************************************
 * @file       ssd1306_gifs.h
 * @author     Abdulrhman Bahaa
 * @brief      This header file includes declarations of OLED display module
 * @date       2024-3-22
 ******************************************************************************
*/
#ifndef SSD1306_GIFS_H
#define	SSD1306_GIFS_H

/* Includes -----------------------------------------------------------------*/
#include <std/mcal_std_types.h>
#include <avr/pgmspace.h>

/* Macro Declarations -------------------------------------------------------*/

/* Macro Functions Declarations ---------------------------------------------*/

/* Data Type Declarations ---------------------------------------------------*/
extern const unsigned char* epd_bitmap_allArray[42];
extern const unsigned char* rotating_bitmap_allArray[121];
extern const unsigned char* eye_bitmap_allArray[59];
extern const unsigned char* key_bitmap_allArray[49];

/* Variables Declarations ---------------------------------------------------*/

/* Functions Declarations ---------------------------------------------------*/

#endif	/* SSD1306_IMAGES_H */
