#include "input.h"

bool key_down(InputState input, Key key) {
	if(input.keyboard[key] == 1) return(true);
	else return(false);
}

bool key_pressed(InputState prev_input, InputState cur_input, Key key) {
	if(!key_down(prev_input, key) and key_down(cur_input, key)) {
		return true;
	}
	else return false;
}