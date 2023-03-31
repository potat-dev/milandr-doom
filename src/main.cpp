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
  Buffer_Text(10, 10, "\xCF\xEE\xF2\xE0\xF2");  // Потат
  DrawBuffer();
  delay_ms(1500);

  while (true) {
    Buffer_Line(8, 8, 127, 8, true);
    Buffer_Line(8, 19, 127, 19, true);
    Buffer_Line(8, 8, 8, 19, true);
    for (j = 0; j < len; j++)
      Buffer_Char(10 + j * CurrentFont->Width, 10,
                  (i + j) % CurrentFont->Count);
    i++;
    DrawBuffer();
    delay_ms(200);
  }
}

int main_old(void) {
  setup();

  while (true) {
    buffer_graphics();
    delay_ms(1000);
    draw_text();
    delay_ms(1000);
  }
}

// --- new code --- //

int main_old_2(void) {
  int i, x, y, clip_height;
  char a[2] = {'0', 0};

  setup();

  while (true) {
    i = 0;

    while (i < 8) {
      fadeScreen(i++ % 8, true);
      DrawBuffer();
      delay_ms(200);
    }

    while (i < 16) {
      y = i++;
      x = y;
      clip_height = max(0, min(y + BMP_GUN_HEIGHT, RENDER_HEIGHT) - y);
      drawBitmap(x, y, bmp_gun_mask, BMP_GUN_WIDTH, clip_height, 0);
      drawBitmap(x, y, bmp_gun_bits, BMP_GUN_WIDTH, clip_height, 1);
      DrawBuffer();
      delay_ms(150);
    }

    drawText(0, 0, "Hello, world!");
    DrawBuffer(false);
    delay_ms(1500);

    drawText(20, 20, "lol kek cheburek =)");
    DrawBuffer();
    delay_ms(1500);

    for (i = 0; i < 10; i++) {
      Buffer_Text(0, 30, "Hello, world!", i);
      a[0] = i + '0';
      Buffer_Text(0, 40, a);
      DrawBuffer();
      delay_ms(200);
    }
  }
}

/// --- doom code --- ///

int main(void) {
  setup();
  while (true) loop();
}

// int main(void) {
//   int i = 0;
//   setup();
//   while (true) {
//     if (input_fire()) i++;
//     Buffer_Char(0, 0, i % 256);
//     Buffer_Text(0, 10, "Hello, world!");
//     DrawBuffer();
//     delay_ms(100);
//   }
// }
