#ifndef __TEXT_H
#define __TEXT_H

#include <stdio.h>
#include <string.h>

#include "../types.h"

// Стили
typedef enum {
  StyleSimple,
  StyleBlink,
  StyleFlipFlop,
  StyleVibratory
} TextStyle;

// Выбранный шрифт для отрисовки текста
extern FONT* CurrentFont;

// Вывод байта на экран
void LCD_PUT_BYTE(u8 x, u8 y, u8 data);

// Вывод символов и строк текущим шрифтом
void Buffer_Char(u8 x, u8 y, u8 ch);
void Buffer_Chars(u8 x, u8 y, uint8_t* str, u8 len);
void Buffer_Text(u8 x, u8 y, char* str, u8 space = 0);
void int2string(u32 u, u8* str);
void Buffer_Int(u8 x, u8 y, u8 u, u8 space);

// Макрокоманда вычисления начала размещения описания символа в таблице описания
// символов. Принимает код символа, адрес структуры описания шрифта. Возвращает
// адрес начала описания символа
#define Get_Char_Data_Addr(ch)                                  \
  (CurrentFont)->pData +                                        \
      (ch) * (CurrentFont)                                      \
                 ->Width*((((CurrentFont)->Height % 8) != 0)    \
                              ? (1 + (CurrentFont)->Height / 8) \
                              : ((CurrentFont)->Height / 8))

#endif
