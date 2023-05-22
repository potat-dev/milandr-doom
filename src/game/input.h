#ifndef _input_h
#define _input_h

#include "../device.h"
#include "../types.h"

// keys
#define SELECT 0xA
#define UP 0xB
#define DOWN 0xC
#define LEFT 0xD
#define RIGHT 0xE

#define INPUT_STATE 0

struct Input {
  bool up;
  bool down;
  bool left;
  bool right;
  bool fire;
};

extern volatile Input input;

// global input state
void update_input(void);
// void init_keys(void); - moved to setup.h

// getters (TODO: remove)
bool input_up(void);
bool input_down(void);
bool input_left(void);
bool input_right(void);
bool input_fire(void);

#endif
