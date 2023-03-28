#define USE_MDR1986VE9x  // for IntelliSense only (auto defined by Keil uVision)

// custom libs
#include "display/text.h"
#include "setup.h"

// system libs
#include "stdbool.h"
#include "stdint.h"

volatile uint32_t _delay_counter = 0;

extern "C" void SysTick_Handler() {
  if (_delay_counter) _delay_counter--;
}

void delay_ms(uint32_t delay) {
  _delay_counter = delay;
  while (_delay_counter)
    ;
}

void display(uint8_t n) {
  ((n & 0x01) ? PORT_SetBits : PORT_ResetBits)(MDR_PORTD, PORT_Pin_10);
  ((n & 0x02) ? PORT_SetBits : PORT_ResetBits)(MDR_PORTD, PORT_Pin_11);
  ((n & 0x04) ? PORT_SetBits : PORT_ResetBits)(MDR_PORTD, PORT_Pin_12);
  ((n & 0x08) ? PORT_SetBits : PORT_ResetBits)(MDR_PORTD, PORT_Pin_13);
  ((n & 0x10) ? PORT_SetBits : PORT_ResetBits)(MDR_PORTD, PORT_Pin_14);
}

void delay(uint32_t ticks) {
  while (ticks--)
    ;
}

void buffer_graphics(void) {
  int i;
  // cross lines
  Buffer_Line(0, 0, 127, 63, true);
  Buffer_Line(127, 0, 0, 63, true);
  // corner circles
  Buffer_Circle(0, 0, 16, true);
  Buffer_Circle(0, 63, 16, true);
  Buffer_Circle(127, 0, 16, true);
  Buffer_Circle(127, 63, 16, true);
  // center circles
  Buffer_Circle(53, 32, 22, true);
  Buffer_Circle(74, 31, 22, true);
  // side lines
  Buffer_Line(22, 11, 22, 52, true);
  Buffer_Line(105, 11, 105, 52, true);
  DrawBuffer();
  // delay(5000000);
  delay_ms(2000);

  // growing circles
  for (i = 0; i < 32; i++) {
    Buffer_Circle(53, 32, i, true);
    Buffer_Circle(74, 31, i, true);
    DrawBuffer();
    delay_ms(50);
  }
}

void draw_text(void) {
  int i = 0, j, len = 16;
  Buffer_Line(8, 8, 127, 8, true);
  Buffer_Line(8, 19, 127, 19, true);
  Buffer_Line(8, 8, 8, 19, true);
  LCD_PUTS(10, 10, "\xCF\xEE\xF2\xE0\xF2");  // Потат
  DrawBuffer();
  delay_ms(1500);

  while (true) {
    Buffer_Line(8, 8, 127, 8, true);
    Buffer_Line(8, 19, 127, 19, true);
    Buffer_Line(8, 8, 8, 19, true);
    for (j = 0; j < len; j++)
      LCD_PUTC(10 + j * CurrentFont->Width, 10, (i + j) % CurrentFont->Count);
    i++;
    DrawBuffer();
    delay_ms(200);
  }
}

int main(void) {
  setup();

  while (true) {
    buffer_graphics();
    delay_ms(1000);
    draw_text();
    delay_ms(1000);
  }
}
