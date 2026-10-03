#pragma once

#include "services/PlayerService.h"

class RankingService {
private:
    const PlayerService& playerService;

public:
    explicit RankingService(const PlayerService& playerService);
    int findRank(const std::string& username) const;
};
