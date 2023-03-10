// custom libs
#include "display/graphics.h"
#include "display/lcd.h"
#include "fonts/font_6x8.h"

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
  while (ticks--);
}

void draw(void) {
  int i = 0;
  LCD_INIT();
  LCD_CLS();

  // test low-level lcd commands
  SetCrystal((LCD_Crystal)0);
  LCD_SET_PAGE(1);
  for (; i < 10; i++) {
    LCD_SET_ADDRESS(i * 2);
    WriteLCD_Data(0xFF);
  }

  // test high-level lcd methods
  LCD_Line(127, 0, 0, 63);
  LCD_Line(64, 32, 127, 63);
  // LCD_Circle(64, 32, 16); // some fuckin bugs
}

void optimal_draw(void) {
  int i, j;
  InitBuffer();
  LCD_INIT();
  LCD_CLS();
	
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			SetBufferByte(j * 16 + i * 2, j, 0xFF);
		}
		DrawBuffer(false);
		delay(1000000);
	}
}

int main(void) {
  int i = 0;
  init_ports();

  // draw();
  optimal_draw();
  
  while (true) {
    display(1 << (i++ % 5));
    delay(500000);
  }
}
