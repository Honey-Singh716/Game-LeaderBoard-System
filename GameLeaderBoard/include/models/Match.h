#pragma once

#include "models/MatchResult.h"

struct Match {
    int matchId;
    std::string player1Username;
    std::string player2Username;
    MatchResult result;
    int player1RatingChange;
    int player2RatingChange;
};
