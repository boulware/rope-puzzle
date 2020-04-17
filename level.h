#ifndef LEVEL_H
#define LEVEL_H

#include "state.h"
#include "game.h"
#include "vec.h"
#include "rect.h"
#include "input.h"

#define MAX_ANCHORS 10
#define MAX_HISTORY_LENGTH 100

enum class Dir : u8 {
	Up = 0,
	Down,
	Left,
	Right,
	None
};



enum class CellType : u8{
	Empty,
	Wall
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

void allocate_array(LocationArray *array, int length, int max_length);
void deallocate_array(LocationArray *array);
bool array_contains(LocationArray array, Veci loc, int *index);
bool array_contains(LocationArray array, Veci loc);
void left_shift_array(LocationArray *array, int start_index, int amount);

Veci cell_in_direction(Veci start, Dir dir);
int taxicab_distance(Veci a, Veci b);
Veci cell_step_towards(Veci start, Veci end, int step_count);

struct Unit {
	Veci loc;
	LocationArray rope;
};

struct GridState {
	bool is_init;
	Unit active_unit;
};

void copy_state(GridState source, GridState *target);
void deallocate(GridState *state);

//void init(GridState *state, Veci active_unit_loc, int rope_max_length, int rope_length);

struct LevelState {
	InputState prev_input;
	Veci grid_size;

	int cell_count = 0;
	CellType *cells;

	int cur_history_index;
	int first_history_index;
	int last_history_index;
	GridState history[MAX_HISTORY_LENGTH];
};

void update_history(LevelState *level, GridState new_state);

CellType cell_at_coordinates(LevelState *level, int x, int y);
CellType cell_at_coordinates(LevelState *level, Veci coordinates);

void init(LevelState *level);
State update_and_render(LevelState *level, InputState input);
bool test_unit_move(LevelState *level, Dir dir);
bool is_valid_target_cell(LevelState *level, Veci loc);
Obj cell_contents(LevelState* level, Veci loc);
Rect cell_rect(LevelState *level, Veci loc);
Veci cell_center_pos(LevelState *level, Veci loc);

#endif