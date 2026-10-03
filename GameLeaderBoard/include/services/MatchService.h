#pragma once

#include "models/Match.h"
#include "services/MatchmakingService.h"
#include "services/PlayerService.h"

#include <optional>
#include <random>
#include <vector>

struct RatingChanges {
    int player1Rating;
    int player2Rating;
};

class MatchService {
private:
    PlayerService& playerService;
    MatchmakingService matchmakingService;
    std::mt19937 generator;
    std::vector<Match> history;
    int nextMatchId = 1;

    MatchResult generateResult();
    RatingChanges getRatingChanges(MatchResult result) const;

public:
    explicit MatchService(PlayerService& playerService);
    MatchService(PlayerService& playerService, unsigned int seed);
    std::optional<Match> playMatch(const std::string& player1Username);
    const std::vector<Match>& getHistory() const;
};
