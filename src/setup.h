#include "display/display.h"
#include "game/input.h"

extern "C" {
#include "core_cm3.h"
// imports for IntelliSense only (can be removed)
// normally this file is auto-linked by the Keil uVision IDE
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32Fx.h"
#include "system_MDR32F9Qx.h"
}

#ifndef SETUP_H
#define SETUP_H

PORT_InitTypeDef PORT_InitStructure;

void init_leds(void) {
  // Enables the RTCHSE clock on PORTC and PORTD
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD, ENABLE);

  // Configure PORTD pins 10..14 for output to switch LEDs on/off
  PORT_InitStructure.PORT_Pin =
      (PORT_Pin_10 | PORT_Pin_11 | PORT_Pin_12 | PORT_Pin_13 | PORT_Pin_14);

  PORT_InitStructure.PORT_OE = PORT_OE_OUT;
  PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
  PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;

  PORT_Init(MDR_PORTD, &PORT_InitStructure);
}

/* --- TODO: configure timers
void Init_Timer1() {
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER1;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN;
  MDR_TIMER1->CNT = 0;
  MDR_TIMER1->PSG = (8000000 / 1000) - 1;
  MDR_TIMER1->ARR = 1000;
  MDR_TIMER1->IE = TIMER_IE_CNT_ARR_EVENT_IE;
  NVIC_EnableIRQ(Timer1_IRQn);
  MDR_TIMER1->CNTRL = TIMER_CNTRL_CNT_EN;
}

void Init_Timer2() {
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER2;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM2_CLK_EN;
  MDR_TIMER2->CNT = 0;
  MDR_TIMER2->PSG = 32000 -1;
  MDR_TIMER2->ARR = 30;
  MDR_TIMER2->IE = TIMER_IE_CNT_ARR_EVENT_IE;
  NVIC_EnableIRQ(Timer2_IRQn);
  MDR_TIMER2->CNTRL = TIMER_CNTRL_CNT_EN;
}
*/

void setup(void) {
  __disable_irq();
  init_leds();
  init_keys();
  setupDisplay();

  SysTick_Config(SystemCoreClock / 1000);

  __enable_irq();
}

#endif  // SETUP_H
