#include "services/RankingService.h"

RankingService::RankingService(const PlayerService& playerService)
    : playerService(playerService) {}

int RankingService::findNaiveRank(const std::string& username) const {
    const Player* target = playerService.findPlayer(username);
    if (target == nullptr) {
        return -1;
    }

    int rank = 1;
    for (const std::string& playerUsername : playerService.getUsernames()) {
        const Player* player = playerService.findPlayer(playerUsername);
        if (player->getWinRate() > target->getWinRate()) {
            ++rank;
        }
    }
    return rank;
}

int RankingService::findOptimizedRank(const std::string& username) const {
    return playerService.findOptimizedRank(username);
}

int RankingService::findRank(const std::string& username) const {
    return findOptimizedRank(username);
}
