// all display related functions

#include <stdio.h>

#include <cmath>

#include "../constants.h"
#include "../game/sprites.h"
#include "../types.h"
#include "graphics.h"
#include "lcd.h"
#include "text.h"

#ifndef __DISPLAY_H
#define __DISPLAY_H

extern uint8_t bit_mask[8];

extern double delta;
extern uint32_t lastFrameTime;

extern uint8_t zbuffer[ZBUFFER_SIZE];

// read bit from PROGMEM
// #define read_bit(b, n) b &pgm_read_byte(bit_mask + n) ? 1 : 0
#define read_bit(b, n) (b & bit_mask[n]) ? true : false

unsigned long millis(void);
void setupDisplay(void);
void fps(void);
bool getGradientPixel(uint8_t x, uint8_t y, uint8_t i);
void fadeScreen(uint8_t intensity, bool color);
// void drawByte(uint8_t x, uint8_t y, uint8_t b);
void drawPixel(int8_t x, int8_t y, bool color, bool raycasterViewport);
void drawVLine(uint8_t x, int8_t start_y, int8_t end_y, uint8_t intensity);
void drawSprite(int8_t x, int8_t y, const uint8_t bitmap[],
                const uint8_t mask[], int16_t w, int16_t h, uint8_t sprite,
                double distance);
void drawChar(int8_t x, int8_t y, char ch);
void drawText(int8_t x, int8_t y, char *txt, uint8_t space = 1);
void drawTextInt(uint8_t x, uint8_t y, uint8_t num);
// void drawText(int8_t x, int8_t y, const __FlashStringHelper txt,
//               uint8_t space);

// -- ported func -- //
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                int16_t h, uint16_t color);
// -- ported func -- //

#endif  // _DISPLAY_H_
