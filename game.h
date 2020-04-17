#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "vec.h"
#include "state.h"
#include "main_menu.h"
#include "campaign.h"
#include "level.h"

struct MainMenuState;

void init_window(SDL_Window **window);

struct Game {
	bool left_mouse_pressed;

	SDL_Window *window;
	Veci mouse_pos;
	State state;

	MainMenuState main_menu_state;
	CampaignState campaign_state;
	LevelState level_state;

	Game();
	void quit();
	void update();
	void key_pressed(SDL_Keysym keysym);
	void mouse_pressed(Uint8 mouse_button, Veci mouse_pos);
};

#endif