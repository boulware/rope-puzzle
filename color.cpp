#include "color.h"
#include "SDL.h"

Uint32 map_color(SDL_Color color) {
	return (color.a << 24) | (color.b << 16) | (color.g << 8) | (color.r << 0);
}