#ifndef CAMPAIGN_H
#define CAMPAIGN_H

namespace campaign {
	const int button_count = 2;
}

struct CampaignState {
	State update_and_render(Game *game);
};

#endif