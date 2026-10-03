#pragma once

#include "models/Player.h"

#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

class PlayerService {
private:
    std::unordered_map<std::string, Player> players;
    std::multimap<int, std::string> playersByRating;

public:
    bool registerPlayer(const std::string& username);
    Player* findPlayer(const std::string& username);
    const Player* findPlayer(const std::string& username) const;
    bool applyRatingChange(const std::string& username, int ratingChange);
    bool removePlayer(const std::string& username);
    std::optional<std::string> findClosestRatedOpponent(
        const std::string& username) const;
    std::vector<std::string> getUsernames() const;
    int size() const;
};
