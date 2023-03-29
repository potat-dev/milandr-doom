// Вывод символов и текста на экран

#include "text.h"

#include "../fonts/font_12x16.h"
#include "../fonts/font_6x8.h"
#include "lcd.h"

// Выбранный шрифт для отрисовки текста
FONT* CurrentFont = &Font_6x8;  // current font

// Вывод байта на экран
void LCD_PUT_BYTE(u8 x, u8 y, u8 data) {
  u32 tmp_data, page, shift_num, shift_num_ex;
  if ((x > MAX_X) | (y > MAX_Y)) return;

  page = y / 8;
  shift_num = y % 8;
  shift_num_ex = 8 - shift_num;

  tmp_data = GetBufferByte(x, page);
  SetBufferByte(x, page, tmp_data | (data << shift_num));
  tmp_data = GetBufferByte(x, page + 1);
  if (shift_num > 0)
    SetBufferByte(x, page + 1, tmp_data | (data >> shift_num_ex));

  /* --- TODO:
  switch (CurrentMethod) {
    case MET_OR:
      WriteLCD_Data(tmp_data | (data << shift_num));
      break;
    case MET_XOR:
      WriteLCD_Data(tmp_data ^ (data << shift_num));
      break;
    case MET_NOT_OR:
      WriteLCD_Data(tmp_data | ((data ^ 0xFF) << shift_num));
      break;
    case MET_NOT_XOR:
      WriteLCD_Data(tmp_data ^ ((data ^ 0xFF) << shift_num));
      break;
    case MET_AND:
      WriteLCD_Data(tmp_data & (0xFF >> shift_num_ex) | (data << shift_num));
      break;
    case MET_FIX:
      WriteLCD_Data((data << shift_num));
      break;
  }
  */
}

// Вывод символов и строк текущим шрифтом

void Buffer_Char(u8 x, u8 y, u8 ch) {
  u32 i, j, line;
  uc8* sym;

  sym = Get_Char_Data_Addr(ch);
  line = CurrentFont->Height / 8;
  if (CurrentFont->Height % 8) line++;

  for (j = 0; j < line; j++)
    for (i = 0; i < CurrentFont->Width; i++)
      LCD_PUT_BYTE(x + i, y + j * 8, sym[i + CurrentFont->Width * j]);
}

void Buffer_Chars(u8 x, u8 y, uint8_t* str, u8 len) {
  u32 i;
  for (i = 0; i < len; i++) Buffer_Char(x + i * CurrentFont->Width, y, str[i]);
}

void int2string(u32 u, u8* str) {
  u8 i;
  u32 dec;
  u8 c;
  dec = 1000000000;
  for (i = 0; i < 10; i++) {
    c = u / dec;
    str[i] = c + 0x30;
    u = u % dec;
    dec = dec / 10;
  }
}

void Buffer_Text(u8 x, u8 y, char* str, u8 space) {
  u32 i;
  for (i = 0; str[i]; i++) Buffer_Char(x + i * (CurrentFont->Width + space), y, str[i]);
}
