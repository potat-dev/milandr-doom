#define USE_MDR1986VE9x  // for IntelliSense only (auto defined by Keil uVision)

// custom libs
// #include "display/text.h"
#include "setup.h"

// system libs
#include "game/doom.h"
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

// -- Main Game Cycle -- //

int main(void) {
  setup();
  while (true) loop();
}
