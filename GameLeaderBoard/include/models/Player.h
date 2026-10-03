#pragma once

#include <string>
#include <cmath>

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

    int getWinRateBasisPoints() const {
        return static_cast<int>(std::lround(getWinRate() * 10000.0));
    }
};
