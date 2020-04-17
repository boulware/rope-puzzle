#include "main_menu.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include "vec.h"
#include "rect.h"
#include "state.h"
#include "constants.h"
#include "game.h"
#include "color.h"
#include "draw.h"
#include "imgui.h"
#include "state.h"

// Returns state that should be transitioned to.
State MainMenuState::update_and_render(Game *game) {
	State new_state = State::None;
	for(int i=0; i<SIZE(main_menu::button_strings); i++) {
		Rect rect = {100, 100+i*(main_menu::button_size.y+30), main_menu::button_size.x, main_menu::button_size.y};
		if(imgui::Button(rect, main_menu::button_strings[i])) {
			new_state = main_menu::button_connected_states[i];
		}
	}

	return new_state;
}