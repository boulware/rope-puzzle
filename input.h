#ifndef INPUT_H
#define INPUT_H

#define Key SDL_Scancode
#define key_a SDL_SCANCODE_A
#define key_d SDL_SCANCODE_D
#define key_s SDL_SCANCODE_S
#define key_w SDL_SCANCODE_W
#define key_backspace SDL_SCANCODE_BACKSPACE
#define key_q SDL_SCANCODE_Q
#define key_e SDL_SCANCODE_E
#define key_comma SDL_SCANCODE_COMMA
#define key_period SDL_SCANCODE_PERIOD

struct InputState {
	int key_count;
	Uint8 *keyboard;
};

bool key_down(InputState input, Key key);
bool key_pressed(InputState prev_input, InputState cur_input, Key key);

#endif