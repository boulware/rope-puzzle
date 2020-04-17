#define SDL_MAIN_HANDLED

#define ASSERT(a) if(!(a)) {*(volatile int*)0 = 0;}
#define SIZE(a) (sizeof(a) / sizeof(a[0]))
#define LAST(a) (a[SIZE(a)-1])
#define and &&
#define or ||

#include "types.h"

#include "math.cpp"
#include "color.cpp"
#include "constants.cpp"
#include "draw.cpp"
#include "game.cpp"
#include "main_menu.cpp"
#include "campaign.cpp"
#include "rect.cpp"
#include "state.cpp"
#include "vec.cpp"
#include "imgui.cpp"
#include "level.cpp"
#include "error.cpp"
#include "input.cpp"

int main(int argc, char *args[]) {
	Game game;
	while(true) {
		game.update();
	}

	return 0;
}