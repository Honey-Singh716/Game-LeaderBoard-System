#pragma once

#include <string>

struct Player {
    std::string username;
    int rating = 400;
    int wins = 0;
    int losses = 0;
    int draws = 0;

    int totalMatchesPlayed() const {
        return wins + losses + draws;
    }

    double getWinRate() const {
        const int totalMatches = totalMatchesPlayed();
        return totalMatches == 0
                   ? 0.0
                   : static_cast<double>(wins) / totalMatches;
    }
};
