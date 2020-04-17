#include "draw.h"

#include "SDL.h"
#include "rect.h"
#include "constants.h"
#include "vec.h"

void render_texture(SDL_Renderer *rend, SDL_Texture *texture, Rect &dst_rect) {
	SDL_RenderCopy(rend, texture, nullptr, &dst_rect);
}

void set_render_color(SDL_Color color) {
	SDL_SetRenderDrawColor(c::rend, color.r, color.g, color.b, color.a);
}

void draw_line(SDL_Renderer *rend, double x0, double y0, double x1, double y1) {
	SDL_RenderDrawLine(rend, (int)x0, (int)y0,
							 (int)x1, (int)y1);
}

void draw_line(SDL_Renderer *rend, Veci start, Veci end) {
	SDL_RenderDrawLine(rend, start.x, start.y, end.x, end.y);
}

void draw_rect(SDL_Renderer *rend, double x, double y, double w, double h) {
	Rect rect = {(int)x,(int)y,(int)w,(int)h};
	SDL_RenderDrawRect(rend, &rect);
}

void draw_triangle(SDL_Renderer *rend, Veci center, int side_length) {
	Veci lower_left = {int(center.x-side_length/2), int(center.y+side_length*0.433)};
	Veci lower_right = {int(center.x+side_length/2), int(center.y+side_length*0.433)};
	Veci top = {center.x, int(center.y-side_length*0.433)};

	draw_line(rend, lower_left, lower_right);
	draw_line(rend, lower_right, top);
	draw_line(rend, top, lower_left);
}