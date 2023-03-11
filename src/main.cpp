// custom libs
#include "display/graphics.h"
#include "display/lcd.h"
#include "display/text.h"

// system libs
#include "stdbool.h"
#include "stdint.h"

PORT_InitTypeDef PORT_InitStructure;

void init_ports(void) {
  /* Enables the RTCHSE clock on PORTC and PORTD */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD, ENABLE);

  /* Configure PORTD pins 10..14 for output to switch LEDs on/off */
  PORT_InitStructure.PORT_Pin =
      (PORT_Pin_10 | PORT_Pin_11 | PORT_Pin_12 | PORT_Pin_13 | PORT_Pin_14);

  PORT_InitStructure.PORT_OE = PORT_OE_OUT;
  PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
  PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;

  PORT_Init(MDR_PORTD, &PORT_InitStructure);
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
  InitBuffer();
  LCD_INIT();
  LCD_CLS();
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
  delay(5000000);
  // growing circles
  for (i = 0; i < 32; i++) {
    Buffer_Circle(53, 32, i, true);
    Buffer_Circle(74, 31, i, true);
    DrawBuffer();
    delay(500000);
  }
}

void draw_text(void) {
  Buffer_Line(8, 8, 127, 8, true);
  Buffer_Line(8, 19, 127, 19, true);
  Buffer_Line(8, 8, 8, 19, true);
  LCD_PUTS(10, 10, "\xCF\xEE\xF2\xE0\xF2");  // Потат
  DrawBuffer();
}

int main(void) {
  int i = 0;
  init_ports();

  buffer_graphics();
  draw_text();

  while (true) {
    display(1 << (i++ % 5));
    delay(500000);
  }
}
