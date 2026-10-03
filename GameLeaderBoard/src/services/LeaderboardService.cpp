#include "services/LeaderboardService.h"

#include "algorithms/MinHeap.h"

#include <algorithm>

LeaderboardService::LeaderboardService(const PlayerService& playerService)
    : playerService(playerService) {}

std::vector<Player> LeaderboardService::getTopK(int k) const {
    std::vector<Player> result;
    if (k <= 0) {
        return result;
    }

    WinRateMinHeap minHeap;
    for (const std::string& username : playerService.getUsernames()) {
        const Player* player = playerService.findPlayer(username);
        minHeap.push({username, player->getWinRate()});
        if (static_cast<int>(minHeap.size()) > k) {
            minHeap.pop();
        }
    }

    while (!minHeap.empty()) {
        result.push_back(*playerService.findPlayer(minHeap.top().username));
        minHeap.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}
