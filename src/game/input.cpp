#include "input.h"

volatile Input input = {false};  // global input state

void update_input(void) {
  if (PORTC_BB[UP] == INPUT_STATE) input.up = true;
  if (PORTC_BB[DOWN] == INPUT_STATE) input.down = true;
  if (PORTC_BB[LEFT] == INPUT_STATE) input.left = true;
  if (PORTC_BB[RIGHT] == INPUT_STATE) input.right = true;
  if (PORTC_BB[SELECT] == INPUT_STATE) input.fire = true;
}

bool input_up(void) { return PORTC_BB[UP] == INPUT_STATE; }

bool input_down(void) { return PORTC_BB[DOWN] == INPUT_STATE; }

bool input_left(void) { return PORTC_BB[LEFT] == INPUT_STATE; }

bool input_right(void) { return PORTC_BB[RIGHT] == INPUT_STATE; }

bool input_fire(void) { return PORTC_BB[SELECT] == INPUT_STATE; }
