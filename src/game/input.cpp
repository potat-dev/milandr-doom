#include "input.h"

Input input = {false};  // global input state

void update_input(void) {
  input.up = PORTC_BB[UP] == INPUT_STATE;
  input.down = PORTC_BB[DOWN] == INPUT_STATE;
  input.left = PORTC_BB[LEFT] == INPUT_STATE;
  input.right = PORTC_BB[RIGHT] == INPUT_STATE;
  input.fire = PORTC_BB[SELECT] == INPUT_STATE;
}

bool input_up(void) { return input.up; }

bool input_down(void) { return input.down; }

bool input_left(void) { return input.left; }

bool input_right(void) { return input.right; }

bool input_fire(void) { return input.fire; }
