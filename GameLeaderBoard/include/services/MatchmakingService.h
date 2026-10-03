#pragma once

#include "services/PlayerService.h"

#include <optional>
#include <string>

class MatchmakingService {
private:
    const PlayerService& playerService;

public:
    explicit MatchmakingService(const PlayerService& playerService);
    std::optional<std::string> findOpponent(
        const std::string& username) const;
};
