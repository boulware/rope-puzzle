#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "game.h"
#include "vec.h"
#include "constants.h"
#include "rect.h"
#include "draw.h"

namespace imgui {
	bool Button(Rect rect, std::string text) {
		// Draw button border (grey if unhovered, green if hovered)
		bool mouse_intersects = point_in_rect(c::game->mouse_pos, rect);
		if(mouse_intersects) {
			set_render_color(c::green);
		}
		else {
			set_render_color(c::grey);
		}
		SDL_RenderDrawRect(c::rend, &rect);

		// Draw button text
		if(text.length() > 0) {
			SDL_Surface *text_surface = TTF_RenderText_Blended(c::font, text.c_str(), c::white);
			SDL_Texture *text_texture = SDL_CreateTextureFromSurface(c::rend, text_surface);

			Rect text_rect = {0, 0, text_surface->w, text_surface->h};
			Veci center_pos = {rect.x + rect.w/2, rect.y + rect.h/2};
			Rect centered_rect = center_rect(text_rect, center_pos);
			render_texture(c::rend, text_texture, centered_rect);

			SDL_FreeSurface(text_surface);
			SDL_DestroyTexture(text_texture);
		}

		if(mouse_intersects && c::game->left_mouse_pressed) return(true);
		else return(false);
	}

	void Label(Veci pos, std::string text, bool center) {
		if(text.length() > 0) {
			SDL_SetRenderDrawColor(c::rend, 255, 255, 255, 255);
			SDL_Surface *text_surface = TTF_RenderText_Blended(c::font, text.c_str(), c::white);
			SDL_Texture *text_texture = SDL_CreateTextureFromSurface(c::rend, text_surface);

			Rect text_rect = {pos.x, pos.y, text_surface->w, text_surface->h};
			if(center == true) {
				text_rect.x -= text_surface->w/2;
				text_rect.y -= text_surface->h/2;
			}
			render_texture(c::rend, text_texture, text_rect);

			SDL_FreeSurface(text_surface);
			SDL_DestroyTexture(text_texture);
		}
	}
}