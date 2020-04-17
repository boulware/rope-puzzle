#ifndef DRAW_H
#define DRAW_H

#include "SDL.h"
#include "rect.h"
#include "vec.h"

void render_texture(SDL_Renderer *rend, SDL_Texture *texture, Rect &dst_rect);
void set_render_color(SDL_Color color);
void draw_line(SDL_Renderer *rend, double x0, double y0, double x1, double y1);
void draw_line(SDL_Renderer *rend, Veci start, Veci end);
void draw_rect(SDL_Renderer *rend, double x, double y, double w, double h);
void draw_triangle(SDL_Renderer *rend, Veci center, int side_length);

#endif