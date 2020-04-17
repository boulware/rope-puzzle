#include "game.h"

#include "stdlib.h"
#include <string>
//using std::string, std::to_string

#include "SDL.h"
#include "SDL_ttf.h"
#include "vec.h"
#include "state.h"
#include "main_menu.h"
#include "constants.h"
#include "imgui.h"
#include "error.h"
#include "input.h"

void init_window(SDL_Window **window) {
	*window = SDL_CreateWindow(	"Traits",
								50, 50,
								1000,800,
								SDL_WINDOW_SHOWN);
}

Game::Game() {
	c::game = this;

	this->left_mouse_pressed = false;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	init_window(&(this->window));
	c::rend = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(c::rend, 0, 0, 0, 255);
	c::font = TTF_OpenFont("font.ttf", 20);
	SDL_GetMouseState(&(this->mouse_pos.x), &(this->mouse_pos.y));

	init(&this->level_state);
	this->state = State::Level;
}

double time_in_s() {
	static const Uint64 start_ticks = SDL_GetTicks();
	static const Uint64 ticks_per_s = SDL_GetPerformanceFrequency();
	return (double)(SDL_GetPerformanceCounter() - start_ticks) / (double)ticks_per_s;
}

//const Uint64 target_frame_ticks = ticks_per_s / 10;
const double dt = 1.0/60.0;
double previous_update_time = time_in_s();
double current_time = time_in_s();
double accumulated_time = 0.0;

void Game::quit() {
	TTF_Quit();
	SDL_Quit();
	exit(0);
}

// Returns true if the game loop should be exited.
void Game::update() {
    double frame_start = time_in_s();
    double frame_time = frame_start - current_time;
    current_time = frame_start;
	accumulated_time += frame_time;

	while(accumulated_time >= dt) {
		accumulated_time -= dt;

		this->left_mouse_pressed = false;

		SDL_GetMouseState(&(this->mouse_pos.x), &(this->mouse_pos.y));

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				this->quit();
			}
			else if(event.type == SDL_KEYDOWN) {
				if(event.key.state == SDL_PRESSED && event.key.repeat == false) {
					this->key_pressed(event.key.keysym);
				}
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN) {
				if(event.button.state == SDL_PRESSED) {
					this->mouse_pressed(event.button.button, this->mouse_pos);
					this->left_mouse_pressed = true;
				}
			}
		}

		InputState input;
		input.keyboard = (Uint8*)SDL_GetKeyboardState(&(input.key_count));

		// Clear screen
		SDL_SetRenderDrawColor(c::rend, 0, 0, 0, 255);
		SDL_RenderClear(c::rend);


		State new_state = State::None;
		switch(this->state) {
			case State::None:
				err("Game is in state: None");
				break;
			case State::MainMenu:
				new_state = this->main_menu_state.update_and_render(this);
				break;
			case State::Campaign:
				new_state = this->campaign_state.update_and_render(this);
				break;
			case State::Level:
				new_state = update_and_render(&(this->level_state), input);
				break;
		}

		switch(new_state) {
			case State::None: break;
			case State::Exit: this->quit(); break;
			default: this->state = new_state; break;
		}

		Veci top_left = {0,0};
		std::string text = std::to_string((int)(0.5 + 1.0 / (frame_start - previous_update_time)));
		imgui::Label(top_left, text);

		previous_update_time = frame_start;
	}

	// Render
	SDL_RenderPresent(c::rend);
}

void Game::key_pressed(SDL_Keysym keysym) {
	if(keysym.scancode == SDL_SCANCODE_ESCAPE) {
		switch(this->state) {
			case State::MainMenu: this->quit(); break;
			case State::Campaign: this->state = State::MainMenu; break;
			case State::Level: this->state = State::Campaign; break;
		}
	}
}

void Game::mouse_pressed(Uint8 mouse_button, Veci mouse_pos) {
	switch(this->state) {
		case State::MainMenu:
			if(mouse_button == SDL_BUTTON_LEFT) {
			}
			break;
	}
}