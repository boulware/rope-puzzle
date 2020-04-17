#include "level.h"

#include <string>

#include "state.h"
#include "game.h"
#include "imgui.h"
#include "vec.h"
#include "draw.h"
#include "math.h"
#include "malloc.h"
#include "input.h"

Veci &LocationArray::operator[](int i) {
	return this->elements[i];
}

bool is_valid_target_cell(LevelState *level, Veci loc) {
	bool cell_exists = loc.x >= 0 and loc.x < level->grid_size.x and loc.y >= 0 and loc.y < level->grid_size.y;
	bool cell_is_occupied = false;
	if(level->active_unit.loc == loc) cell_is_occupied = true;
	if(array_contains(level->anchors, loc, nullptr)) cell_is_occupied = true;
	// for(int i=0; i<level->anchors.length; i++) {
	// 	if(level->anchors[i] == loc) {
	// 		cell_is_occupied = true;
	// 		break;
	// 	}
	// }

	if(cell_exists and !cell_is_occupied) {
		return(true);
	}
	else return(false);
}

Rect cell_rect(LevelState *level, Veci loc) {
	return {c::grid_origin.x + loc.x*c::cell_size,
			c::grid_origin.y + loc.y*c::cell_size,
			c::cell_size,
			c::cell_size};
}

Veci cell_center_pos(LevelState *level, Veci loc) {
	Rect rect = cell_rect(level, loc);
	return center(rect);
}

Veci cell_in_direction(Veci start, Dir dir) {
	switch(dir) {
		case Dir::Up: return {start.x, start.y-1};
		case Dir::Down: return {start.x, start.y+1};
		case Dir::Left: return {start.x-1, start.y};
		case Dir::Right: return {start.x+1, start.y};
		default: return start;
	}
}

Obj cell_contents(LevelState* level, Veci loc) {
	Unit &active_unit = level->active_unit;
	LocationArray &rope = active_unit.rope;

	if(is_valid_target_cell(level, loc) == false) return Obj::None;

	if(active_unit.loc == loc) return Obj::Unit;

	for(int i=0; i<rope.length; i++) {
		if(rope[i] == loc) {
			return Obj::Rope;
		}
	}

	for(int i=0; i<level->anchors.length; i++) {
		if(level->anchors[i] == loc) return Obj::Anchor;
	}

	return Obj::None;
}

int taxicab_distance(Veci a, Veci b) {
	return abs(a.x-b.x)+abs(a.y-b.y);
}

Veci cell_step_towards(Veci start, Veci end, int step_count) {
	// Not implemented for cells not aligned on at least one of the axes.
	ASSERT(start.x == end.x or start.y == end.y);

	if(step_count >= taxicab_distance(start, end)) {
		return end;
	}

	if(start.x == end.x) {
		return {start.x, start.y + step_count*sign(end.y-start.y)};
	}
	else if(start.y == end.y) {
		return {start.x + step_count*sign(end.x-start.x), start.y};
	}
	else {
		return start;
	}
}

bool array_contains(LocationArray array, Veci loc, int *index) {
	for(int i=0; i<array.length; i++) {
		if(array[i] == loc) {
			if(index != nullptr) *index = i;
			return true;
		}
	}

	return false;
}

void left_shift_array(LocationArray &array, int start_index, int amount) {
	ASSERT(start_index >= 0 and start_index < array.length);

	for(int i=start_index+amount; i<array.max_length; i++) {
		array[i-amount] = array[i];
	}
	array.length -= amount;
}

void try_unit_move_old(LevelState *level, Dir dir) {
	Unit &active_unit = level->active_unit;
	LocationArray &rope = active_unit.rope;

	Veci target_loc = cell_in_direction(active_unit.loc, dir);

	// In Case 1, we just skip everything below.
	// Case 2 through 6
	if(!is_valid_target_cell(level, target_loc)) {
		return;
	}

	int target_loc_rope_index;
	if(array_contains(rope, target_loc, &target_loc_rope_index) == false) {
		if(rope.length < rope.max_length) {
			// Case 2
			rope[rope.length] = active_unit.loc;
			rope.length += 1;
		}
		else {
			// Case 3
			if(!array_contains(level->anchors, rope[0], nullptr)) {
				left_shift_array(rope, 0, 1);
				rope.length += 1;
				rope[rope.max_length-1] = active_unit.loc;
			}
			else {
				// Rope is at full length and connected to an anchor so
				// movement is impossible (not really, the rope can be pulled
				// through other cells)

				bool rope_shifted = false;

				for(int i=rope.max_length-1; i>=0; i--) {
					if(rope_shifted == true) break;
					for(int j=i; j<rope.max_length; j++) {
						int length_along_rope = j-i;
						int shortest_distance = taxicab_distance(rope[i], rope[j]);
						bool locations_x_aligned = (rope[i].x == rope[j].x);
						bool locations_y_aligned = (rope[i].y == rope[j].y);

						bool rope_between_endpoints = false;
						if(locations_x_aligned == true) {
							for(int r=0; r<i; r++) {
								if(rope[r].x == rope[i].x and in_range(rope[r].y, rope[i].y, rope[j].y)) {
									rope_between_endpoints = true;
									break;
								}
							}
						}
						else if(locations_y_aligned == true) {
							for(int r=0; r<i; r++) {
								if(rope[r].y == rope[i].y and in_range(rope[r].x, rope[i].x, rope[j].x)) {
									rope_between_endpoints = true;
									break;
								}
							}
						}

						if((length_along_rope != shortest_distance) and (locations_x_aligned or locations_y_aligned) and !rope_between_endpoints) {

							Veci start_i = rope[i];
							Veci end_i = rope[j];
							left_shift_array(rope, j-2, 2);
							rope[rope.length] = active_unit.loc;
							rope.length += 1;

							// between (inclusive) i+1 and j-3, we need to
							// shift them to the shortest path between
							// i and j. Because locations_are_linear is guaranteed
							// here, we know that any sequential step between i and j,
							// will be purely linear, so we can just sequentially
							// step from i to j, filling in the indices.

							int step_size = 1;
							for(int k=i+1; k<=j-3; k++) {
								rope[k] = cell_step_towards(start_i, end_i, step_size);
								step_size += 1;
							}

							rope_shifted = true;
							break;
						}
					}
				}

				if(rope_shifted == false) {
					for(int i=rope.max_length-1; i>=0; i--) {
						if(cell_contents(level, cell_in_direction(rope[i], dir)) == Obj::Rope) {
							// Here we've assumed (unsupported) that the rope it sees is the
							// piece immediately connected to it, so let's assert it.
							int other_rope_index;
							array_contains(rope, cell_in_direction(rope[i], dir), &other_rope_index);
							ASSERT(other_rope_index == i-1);

							for(int j=rope.max_length-1; j>=i; j--) {
								rope[j] = cell_in_direction(rope[j], dir);
							}
							left_shift_array(rope, i-1, 1);

							rope_shifted = true;
							break;
						}
					}
				}

				if(rope_shifted == false) {
					return;
				}
			}
		}
	}
	else {
		if(target_loc_rope_index == rope.length-1) {
			// Case 4
			rope.length -= 1;
		}
		else {
			// Case 5+6
			rope.length -= rope.length-target_loc_rope_index;
		}
	}
	active_unit.loc = target_loc; // Move unit to target loc
}

void try_unit_move(LevelState *level, Dir dir) {

}

void init(LevelState *level) {
	// Check to see how many keys there are in a typical GetKeyboardState()
	// so we can allocate memory for prev_input
	SDL_GetKeyboardState(&(level->prev_input.key_count));
	level->prev_input.keyboard = (Uint8*)malloc(level->prev_input.key_count*sizeof(Uint8*));
	level->grid_size = {10,10};

	level->anchors.elements = (Veci*)malloc(MAX_ANCHORS*sizeof(Veci));
	level->anchors.max_length = MAX_ANCHORS;
	level->anchors.length = 1;
	level->anchors[0] = {4,4};

	// Aliases
	Unit &active_unit = level->active_unit;
	LocationArray &rope = active_unit.rope;

	active_unit.loc = {3,4};
	rope.max_length = 11;
	rope.elements = (Veci*)malloc(rope.max_length*sizeof(Veci));
	rope.length = 0;

	// Test rope for anchor mechanics
	rope.length = 1;
	rope[0] = {4,4};
}

State update_and_render(LevelState *level, InputState input) {
	// If this assertion fails, we've made a bad assumption that key_count is invariant.
	ASSERT(input.key_count == level->prev_input.key_count);

	Unit &active_unit = level->active_unit;
	LocationArray &rope = active_unit.rope;

	if(key_pressed(level->prev_input, input, key_w)) try_unit_move(level, Dir::Up);
	if(key_pressed(level->prev_input, input, key_a)) try_unit_move(level, Dir::Left);
	if(key_pressed(level->prev_input, input, key_s)) try_unit_move(level, Dir::Down);
	if(key_pressed(level->prev_input, input, key_d)) try_unit_move(level, Dir::Right);

	for(int i=0; i<input.key_count; i++) {
		level->prev_input.keyboard[i] = input.keyboard[i];
	}

	// Draw grid outline
	set_render_color(c::white);
	for(int x=0; x<level->grid_size.x + 1; x++)
	{
		int x_adjusted = c::grid_origin.x + c::cell_size*x;
		draw_line(c::rend, x_adjusted, c::grid_origin.y, x_adjusted, c::grid_origin.y + c::cell_size*(level->grid_size.y));
	}
	for(int y=0; y<level->grid_size.y + 1; y++) {
		int y_adjusted = c::grid_origin.y + c::cell_size*y;
		draw_line(c::rend, c::grid_origin.x, y_adjusted, c::grid_origin.x + c::cell_size*level->grid_size.x, y_adjusted);
	}

	// Draw unit
	Veci cell_center = cell_center_pos(level, active_unit.loc);
	int unit_size = 30;
	set_render_color(c::red);
	draw_rect(c::rend, cell_center.x - unit_size/2, cell_center.y - unit_size/2, unit_size, unit_size);

	// Draw rope
	if(rope.length != 0) {
		for(int i=0; i<rope.length-1; i++) {
			Veci start_center = cell_center_pos(level, rope[i]);
			Veci end_center = cell_center_pos(level, rope[i+1]);

			draw_line(c::rend, start_center, end_center);
		}

		// Draw a line between the last rope piece and the unit.
		Veci last_piece_center_pos = cell_center_pos(level, rope[rope.length-1]);
		Veci unit_center_pos = cell_center_pos(level, active_unit.loc);
		draw_line(c::rend, last_piece_center_pos, unit_center_pos);
	}

	// Draw anchors
	set_render_color(c::green);
	for(int i=0; i<level->anchors.length; i++) {
		draw_triangle(c::rend, cell_center_pos(level, level->anchors[i]), 20);
	}

	imgui::Label({100,0}, std::to_string(rope.length), false);

	return State::None;
}