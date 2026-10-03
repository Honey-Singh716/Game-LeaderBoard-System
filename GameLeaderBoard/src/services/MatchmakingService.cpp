#include "services/MatchmakingService.h"

MatchmakingService::MatchmakingService(const PlayerService& playerService)
    : playerService(playerService) {}

std::optional<std::string> MatchmakingService::findOpponent(
    const std::string& username) const {
    return playerService.findClosestRatedOpponent(username);
}
