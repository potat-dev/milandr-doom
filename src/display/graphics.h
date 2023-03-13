// Вывод графических примитивов на LCD-экран

#ifndef __GL_H
#define __GL_H

#include "../types.h"

void Buffer_Line(u32 x1, u32 y1, u32 x2, u32 y2, bool c);
void Buffer_Circle(u32 xc, u32 yc, u32 r, bool c);
void Buffer_Sprite(u32 x, u32 w, const u8 *data);

#endif /* __GL_H */
