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
	if(level->history[level->cur_history_index].active_unit.loc == loc) cell_is_occupied = true;
	//if(array_contains(level->anchors, loc, nullptr)) cell_is_occupied = true;
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
	Unit &active_unit = level->history[level->cur_history_index].active_unit;
	LocationArray &rope = active_unit.rope;

	if(is_valid_target_cell(level, loc) == false) return Obj::None;

	if(active_unit.loc == loc) return Obj::Unit;

	for(int i=0; i<rope.length; i++) {
		if(rope[i] == loc) {
			return Obj::Rope;
		}
	}

	// for(int i=0; i<level->anchors.length; i++) {
	// 	if(level->anchors[i] == loc) return Obj::Anchor;
	// }

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

bool array_contains(LocationArray array, Veci loc) {
	return array_contains(array, loc, nullptr);
}

void left_shift_array(LocationArray &array, int start_index, int amount) {
	ASSERT(start_index >= 0 and start_index < array.length);

	for(int i=start_index+amount; i<array.max_length; i++) {
		array[i-amount] = array[i];
	}
	array.length -= amount;
}

// void try_unit_move_old(LevelState *level, Dir dir) {
// 	Unit &active_unit = level->active_unit;
// 	LocationArray &rope = active_unit.rope;

// 	Veci target_loc = cell_in_direction(active_unit.loc, dir);

// 	// In Case 1, we just skip everything below.
// 	// Case 2 through 6
// 	if(!is_valid_target_cell(level, target_loc)) {
// 		return;
// 	}

// 	int target_loc_rope_index;
// 	if(array_contains(rope, target_loc, &target_loc_rope_index) == false) {
// 		if(rope.length < rope.max_length) {
// 			// Case 2
// 			rope[rope.length] = active_unit.loc;
// 			rope.length += 1;
// 		}
// 		else {
// 			// Case 3
// 			if(!array_contains(level->anchors, rope[0], nullptr)) {
// 				left_shift_array(rope, 0, 1);
// 				rope.length += 1;
// 				rope[rope.max_length-1] = active_unit.loc;
// 			}
// 			else {
// 				// Rope is at full length and connected to an anchor so
// 				// movement is impossible (not really, the rope can be pulled
// 				// through other cells)

// 				bool rope_shifted = false;

// 				for(int i=rope.max_length-1; i>=0; i--) {
// 					if(rope_shifted == true) break;
// 					for(int j=i; j<rope.max_length; j++) {
// 						int length_along_rope = j-i;
// 						int shortest_distance = taxicab_distance(rope[i], rope[j]);
// 						bool locations_x_aligned = (rope[i].x == rope[j].x);
// 						bool locations_y_aligned = (rope[i].y == rope[j].y);

// 						bool rope_between_endpoints = false;
// 						if(locations_x_aligned == true) {
// 							for(int r=0; r<i; r++) {
// 								if(rope[r].x == rope[i].x and in_range(rope[r].y, rope[i].y, rope[j].y)) {
// 									rope_between_endpoints = true;
// 									break;
// 								}
// 							}
// 						}
// 						else if(locations_y_aligned == true) {
// 							for(int r=0; r<i; r++) {
// 								if(rope[r].y == rope[i].y and in_range(rope[r].x, rope[i].x, rope[j].x)) {
// 									rope_between_endpoints = true;
// 									break;
// 								}
// 							}
// 						}

// 						if((length_along_rope != shortest_distance) and (locations_x_aligned or locations_y_aligned) and !rope_between_endpoints) {

// 							Veci start_i = rope[i];
// 							Veci end_i = rope[j];
// 							left_shift_array(rope, j-2, 2);
// 							rope[rope.length] = active_unit.loc;
// 							rope.length += 1;

// 							// between (inclusive) i+1 and j-3, we need to
// 							// shift them to the shortest path between
// 							// i and j. Because locations_are_linear is guaranteed
// 							// here, we know that any sequential step between i and j,
// 							// will be purely linear, so we can just sequentially
// 							// step from i to j, filling in the indices.

// 							int step_size = 1;
// 							for(int k=i+1; k<=j-3; k++) {
// 								rope[k] = cell_step_towards(start_i, end_i, step_size);
// 								step_size += 1;
// 							}

// 							rope_shifted = true;
// 							break;
// 						}
// 					}
// 				}

// 				if(rope_shifted == false) {
// 					for(int i=rope.max_length-1; i>=0; i--) {
// 						if(cell_contents(level, cell_in_direction(rope[i], dir)) == Obj::Rope) {
// 							// Here we've assumed (unsupported) that the rope it sees is the
// 							// piece immediately connected to it, so let's assert it.
// 							int other_rope_index;
// 							array_contains(rope, cell_in_direction(rope[i], dir), &other_rope_index);
// 							ASSERT(other_rope_index == i-1);

// 							for(int j=rope.max_length-1; j>=i; j--) {
// 								rope[j] = cell_in_direction(rope[j], dir);
// 							}
// 							left_shift_array(rope, i-1, 1);

// 							rope_shifted = true;
// 							break;
// 						}
// 					}
// 				}

// 				if(rope_shifted == false) {
// 					return;
// 				}
// 			}
// 		}
// 	}
// 	else {
// 		if(target_loc_rope_index == rope.length-1) {
// 			// Case 4
// 			rope.length -= 1;
// 		}
// 		else {
// 			// Case 5+6
// 			rope.length -= rope.length-target_loc_rope_index;
// 		}
// 	}
// 	active_unit.loc = target_loc; // Move unit to target loc
// }

bool test_unit_move(LevelState *level, Dir dir) {
	Unit &active_unit = level->history[level->cur_history_index].active_unit;
	LocationArray &rope = active_unit.rope;

	Veci target_loc = cell_in_direction(active_unit.loc, dir);
	CellType target_cell_type = cell_at_coordinates(level, target_loc);

	if(target_cell_type == CellType::Wall) return false;
	if(array_contains(rope, target_loc)) return false;

	return true;
}

void allocate_array(LocationArray &array, int length, int max_length) {
	array.length = length;
	array.max_length = max_length;
	array.elements = (Veci*)malloc(max_length*sizeof(Veci));
}

void init(LevelState *level) {
	// Check to see how many keys there are in a typical GetKeyboardState()
	// so we can allocate memory for prev_input
	SDL_GetKeyboardState(&(level->prev_input.key_count));
	level->prev_input.keyboard = (Uint8*)malloc(level->prev_input.key_count*sizeof(Uint8*));

	level->cur_history_index = 0;
	level->first_history_index = 0;
	level->last_history_index = 0;
	for(int i=0; i<SIZE(level->history); i++) {
		level->history[i].is_init = false;
	}

	// Aliases
	Unit &active_unit = level->history[level->cur_history_index].active_unit;
	LocationArray &rope = active_unit.rope;

	allocate_array(rope, 0, 10);
	level->history[0].is_init = true;

	// Load level from file
	// This inits grid_size, active_unit.loc
	{
		FILE *file = fopen("level.dat", "r");
		char line[100];
		bool first_line = true;
		int level_width = 0;
		int level_height = 0;
		bool found_unit = false;

		int cur_size = 0;
		int cur_index = 0;
		// First read through file to get level dimensions.
		while(!feof(file)) {
			char *result = fgets(line, SIZE(line), file);
			if(first_line == true) {
				for(int i=0; i<SIZE(line); i++) {
					if(line[i] != '\0' and line[i] != '\n') {
						level_width += 1;
					}
					else {
						cur_size = level_width*sizeof(CellType);
						level->cells = (CellType*)malloc(cur_size);
						level->cell_count = cur_size;
						break;
					}
				}
			}

			if(result != nullptr) level_height += 1;

			if(first_line == false) {
				cur_size += level_width*sizeof(CellType);
				level->cell_count = cur_size;
				level->cells = (CellType*)realloc(level->cells, cur_size);
			}

			int row_width = 0;
			for(int i=0; i<SIZE(line); i++) {
				if(line[i] == '\0' or line[i] == '\n') break;

				ASSERT(cur_index < cur_size/sizeof(CellType));

				if(line[i] == 'X') level->cells[cur_index++] = CellType::Wall;
				else if(line[i] == ' ') level->cells[cur_index++] = CellType::Empty;
				else if(line[i] == 'O') {
					level->cells[cur_index] = CellType::Empty;
					active_unit.loc = {cur_index%level_width, cur_index/level_width};
					cur_index += 1;
					found_unit = true;
				}

				row_width += 1;
			}

			if(first_line == true) first_line = false;

			ASSERT(row_width == level_width);
		}

		level->grid_size = {level_width, level_height};
	}

	allocate_array(rope, 0, 5);
}

CellType cell_at_coordinates(LevelState *level, int x, int y) {
	return level->cells[x + y*level->grid_size.x];
}

void deallocate_array(LocationArray *array) {
	free(array->elements);
	array->max_length = 0;
	array->length = 0;
}

void deallocate(GridState *state) {
	deallocate_array(&state->active_unit.rope);
}

void update_history(LevelState *level, GridState new_state) {
	level->cur_history_index = (level->cur_history_index+1) % SIZE(level->history);
	level->last_history_index = level->cur_history_index;
	if(level->last_history_index == level->first_history_index) {
		level->first_history_index = (level->first_history_index+1) % SIZE(level->history);
	}

	// Alias
	GridState &current_state = level->history[level->cur_history_index];

	if(current_state.is_init == true) deallocate(&current_state);
	current_state = new_state;
}

CellType cell_at_coordinates(LevelState *level, Veci coordinates) {
	return cell_at_coordinates(level, coordinates.x, coordinates.y);
}

void copy_state(GridState source, GridState *target) {
	target->active_unit.loc = source.active_unit.loc;

	LocationArray &source_rope = source.active_unit.rope;
	LocationArray &target_rope = target->active_unit.rope;
	allocate_array(target_rope, source_rope.length, source_rope.max_length);
	for(int i=0; i<source_rope.length; i++) {
		target_rope[i] = source_rope[i];
	}
}

State update_and_render(LevelState *level, InputState input) {
	// If this assertion fails, we've made a bad assumption that key_count is invariant.
	ASSERT(input.key_count == level->prev_input.key_count);

	Unit &active_unit = level->history[level->cur_history_index].active_unit;
	LocationArray &rope = active_unit.rope;

	if(key_pressed(level->prev_input, input, key_comma)) {
		// Go back one step in the history
		if(level->cur_history_index == level->first_history_index) {
			// We're already as far back as we can go. Do nothing.
		}
		else {
			level->cur_history_index = (level->cur_history_index-1)%SIZE(level->history);
		}
	}
	if(key_pressed(level->prev_input, input, key_period)) {
		// Go forward one step in the history
		if(level->cur_history_index == level->last_history_index) {
			// We're already as far back as we can go. Do nothing.
		}
		else {
			level->cur_history_index = (level->cur_history_index+1)%SIZE(level->history);
		}
	}

	Dir try_move_direction = Dir::None;
	if(key_pressed(level->prev_input, input, key_w)) try_move_direction = Dir::Up;
	if(key_pressed(level->prev_input, input, key_a)) try_move_direction = Dir::Left;
	if(key_pressed(level->prev_input, input, key_s)) try_move_direction = Dir::Down;
	if(key_pressed(level->prev_input, input, key_d)) try_move_direction = Dir::Right;

	if(try_move_direction != Dir::None) {
		// @Note: I changed it to test_unit_move() only gets called once, but that
		// makes it feel weird when moving fast.
		bool move_success = test_unit_move(level, try_move_direction);
		if(move_success == true) {
			GridState new_state;
			copy_state(level->history[level->cur_history_index], &new_state);
			Unit &new_active_unit = new_state.active_unit;
			LocationArray &new_rope = new_active_unit.rope;

			if(new_rope.length < new_rope.max_length) {
				new_rope[new_rope.length] = new_active_unit.loc;
				new_rope.length += 1;
			}
			else {
				left_shift_array(new_rope, 0, 1);
				new_rope.length += 1;
				new_rope[new_rope.length-1] = new_active_unit.loc;
			}
			new_active_unit.loc = cell_in_direction(new_active_unit.loc, try_move_direction);

			update_history(level, new_state);
		}
	}

	for(int i=0; i<input.key_count; i++) {
		level->prev_input.keyboard[i] = input.keyboard[i];
	}

	// Draw grid outline
	set_render_color(c::white);
	for(int y=0; y<level->grid_size.y; y++) {
		for(int x=0; x<level->grid_size.x; x++) {
			if(cell_at_coordinates(level, x, y) != CellType::Wall) continue;
			draw_rect(c::rend, c::grid_origin.x + x*c::cell_size,
							   c::grid_origin.y + y*c::cell_size,
							   c::cell_size+1, c::cell_size+1);
		}
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
	// set_render_color(c::green);
	// for(int i=0; i<level->anchors.length; i++) {
	// 	draw_triangle(c::rend, cell_center_pos(level, level->anchors[i]), 20);
	// }

	imgui::Label({100,0}, std::to_string(rope.length), false);

	return State::None;
}