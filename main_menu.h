#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "SDL.h"
#include <string>

#include "vec.h"
#include "state.h"
#include "game.h"

struct Game;

namespace main_menu {
	const int button_count = 2;
	const Veci button_size = {200,100};
	const std::string button_strings[button_count] = {"Game", "Exit"};
	const State button_connected_states[button_count] = {State::Campaign, State::Exit};
}

struct MainMenuState {
	State update_and_render(Game *game);
};

#endif