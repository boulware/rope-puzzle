#ifndef RECT_H
#define RECT_H

#include "SDL.h"
#include "vec.h"

typedef SDL_Rect Rect;

bool point_in_rect(Veci point, Rect rect);
Veci center(Rect rect);
Rect center_rect(Rect rect, Veci point);

#endif