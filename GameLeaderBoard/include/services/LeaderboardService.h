#pragma once

#include "services/PlayerService.h"
#include "models/Player.h"

#include <vector>

class LeaderboardService {
private:
    const PlayerService& playerService;

public:
    explicit LeaderboardService(const PlayerService& playerService);
    std::vector<Player> getTopK(int k) const;
};
