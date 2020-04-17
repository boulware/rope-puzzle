#include "constants.h"

#include "SDL.h"
#include "game.h"
#include "vec.h"

namespace c {
	Game *game = nullptr;
	SDL_Renderer *rend = nullptr;

	SDL_Color black = {0,0,0,255};
	SDL_Color white = {255,255,255};
	SDL_Color red = {255,0,0,255};
	SDL_Color green = {0,255,0,255};
	SDL_Color blue = {0,0,255,255};
	SDL_Color yellow = {255,255,0,255};
	SDL_Color cyan = {0,255,255,255};
	SDL_Color magenta = {255,0,255,255};
	SDL_Color grey = {128,128,128,255};

	SDL_PixelFormatEnum pixel_format = SDL_PIXELFORMAT_RGBA32;
	TTF_Font *font = nullptr;

	const Veci grid_origin = {100,100};
	const int cell_size = 60;
	// const Veci grid_size = {5,5};
}