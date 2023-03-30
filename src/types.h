#ifndef __types_h__
#define __types_h__

#include <math.h>
#include <stdint.h>

#include "constants.h"
#include "device.h"

typedef signed long s32;
typedef signed short s16;
typedef signed char s8;

typedef signed long const sc32;
typedef signed short const sc16;
typedef signed char const sc8;

typedef volatile signed long vs32;
typedef volatile signed short vs16;
typedef volatile signed char vs8;

typedef volatile signed long const vsc32;
typedef volatile signed short const vsc16;
typedef volatile signed char const vsc8;

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef unsigned long const uc32;
typedef unsigned short const uc16;
typedef unsigned char const uc8;

typedef volatile unsigned long vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char vu8;

typedef volatile unsigned long const vuc32;
typedef volatile unsigned short const vuc16;
typedef volatile unsigned char const vuc8;

// font type

typedef struct tag_FONT {
  u32 Height; /* Высота символа в пикселях. */
  u32 Width;  /* Ширина символа в пикселях. */
  u32 Count; /* Число символов в шрифте минус один. */
  u8* pData; /* Адрес таблицы с описанием символов шрифта. */
} FONT;

// Doom types //

#define UID_null 0

// Entity types (legend applies to level.h)
#define E_FLOOR 0x0       // . (also null)
#define E_WALL 0xF        // #
#define E_PLAYER 0x1      // P
#define E_ENEMY 0x2       // E
#define E_DOOR 0x4        // D
#define E_LOCKEDDOOR 0x5  // L
#define E_EXIT 0x7        // X

// collectable entities >= 0x8
#define E_MEDIKIT 0x8   // M
#define E_KEY 0x9       // K
#define E_FIREBALL 0xA  // not in map

typedef uint16_t UID;
typedef uint8_t EType;

struct Coords {
  double x;
  double y;
};

UID create_uid(EType type, uint8_t x, uint8_t y);
EType uid_get_type(UID uid);

struct Coords create_coords(double x, double y);
uint8_t coords_distance(struct Coords* a, struct Coords* b);

// helpers

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#endif /* __TYPES_H */
