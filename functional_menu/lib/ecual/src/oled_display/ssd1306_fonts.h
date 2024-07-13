/*
     MIT License
 
     Copyright (c) 2018, Alexey Dynda
 
     Permission is hereby granted, free of charge, to any person obtaining a copy
     of this software and associated documentation files (the "Software"), to deal
     in the Software without restriction, including without limitation the rights
     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
     copies of the Software, and to permit persons to whom the Software is
     furnished to do so, subject to the following conditions:
 
     The above copyright notice and this permission notice shall be included in all
     copies or substantial portions of the Software.
 
     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
     SOFTWARE.
 */
 #ifndef SSD1306_FONTS_H
 #define SSD1306_FONTS_H

#include <atmega328.h> 
#include <avr/pgmspace.h>

extern const uint8_t ssd1306xled_font6x8[] PROGMEM;
extern const uint8_t ssd1306xled_font8x16[] PROGMEM;
extern const uint8_t courier_new_font11x16_digits[] PROGMEM;
extern const uint8_t comic_sans_font24x32_123[] PROGMEM;
 
 #endif // SSD1306_FONTS_H