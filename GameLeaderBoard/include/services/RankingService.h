#pragma once

#include "services/PlayerService.h"

class RankingService {
private:
    const PlayerService& playerService;

public:
    explicit RankingService(const PlayerService& playerService);
    int findNaiveRank(const std::string& username) const;
    int findOptimizedRank(const std::string& username) const;
    int findRank(const std::string& username) const;
};
