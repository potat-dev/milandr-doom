// custom libs
#include "display/lcd.h"
#include "fonts/font_6x8.h"
#include "types.h"

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
  while (ticks--) {
  }
}

int main(void) {
  int i = 0;
  init_ports();
  LCD_INIT();
  LCD_CLS();

  SetCrystal((LCD_Crystal)0);
  LCD_SET_PAGE(1);
  for (; i < 10; i++) {
    LCD_SET_ADDRESS(i * 2);
    WriteLCD_Data(0xFF);
  }

  i = 0;
  while (true) {
    display(1 << (i++ % 5));
    delay(500000);
  }
}
