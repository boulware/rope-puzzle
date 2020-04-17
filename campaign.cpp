#include "campaign.h"
#include "game.h"
#include "state.h"
#include "imgui.h"

State CampaignState::update_and_render(Game *game) {
	State new_state = State::None;

	Rect rect = {300,300,200,100};
	if(imgui::Button(rect, "Level 1")) {
		new_state = State::Level;
	}

	return new_state;
}