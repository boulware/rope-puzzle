#ifndef LEVEL_H
#define LEVEL_H

#include "state.h"
#include "game.h"
#include "vec.h"
#include "rect.h"
#include "input.h"

#define MAX_ANCHORS 10

enum class Dir : u8 {
	Up = 0,
	Down,
	Left,
	Right,
	None
};

enum class Obj {
	None,
	Unit,
	Rope,
	Anchor
};

struct LocationArray {
	u32 max_length;
	u32 length;
	Veci *elements;

	Veci &operator[](int i);
};

bool array_contains(LocationArray array, Veci loc, int *index);
void left_shift_array(LocationArray &array, int start_index, int amount);

Veci cell_in_direction(Veci start, Dir dir);
int taxicab_distance(Veci a, Veci b);
Veci cell_step_towards(Veci start, Veci end, int step_count);

// struct Rope {
// 	int length;
// 	int laid_down_length;
// 	Veci *segments;
// };

struct Unit {
	Veci loc;
	LocationArray rope;
};

struct LevelState {
	InputState prev_input;
	Veci grid_size;
	Unit active_unit;
	LocationArray anchors;
};

void init(LevelState *level);
State update_and_render(LevelState *level, InputState input);
void try_unit_move(LevelState *level, Veci target_loc);
bool is_valid_target_cell(LevelState *level, Veci loc);
Obj cell_contents(LevelState* level, Veci loc);
Rect cell_rect(LevelState *level, Veci loc);
Veci cell_center_pos(LevelState *level, Veci loc);

#endif