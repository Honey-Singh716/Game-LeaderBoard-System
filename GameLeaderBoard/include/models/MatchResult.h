#pragma once

#include <string>

enum class MatchResult {
    PLAYER_1_WIN,
    PLAYER_2_WIN,
    DRAW
};

std::string toString(MatchResult result);
