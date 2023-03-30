#include "entities.h"

#include "../types.h"

Player create_player(uint8_t x, uint8_t y) {
  Coords pos = create_coords((double)x + 0.5, (double)y + 0.5);
  Coords dir = create_coords(1, 0);
  Coords plane = create_coords(0, -0.66);
  Player p = {pos, dir, plane, 0, 100, 0};
  return p;
}

Entity create_entity(uint8_t type, uint8_t x, uint8_t y, uint8_t initialState,
                     uint8_t initialHealth) {
  UID uid = create_uid(type, x, y);
  Coords pos = create_coords((double)x + .5, (double)y + .5);
  Entity new_entity = {uid, pos, initialState, initialHealth, 0, 0};
  return new_entity;
}

StaticEntity crate_static_entity(UID uid, uint8_t x, uint8_t y, bool active) {
  StaticEntity se = {uid, x, y, active};
  return se;
}
