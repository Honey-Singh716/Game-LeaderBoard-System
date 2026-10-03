#pragma once

#include "algorithms/AVLRankTree.h"
#include "models/Player.h"

#include <map>
#include <optional>
#include <unordered_map>
#include <vector>

class PlayerService {
private:
    std::unordered_map<std::string, Player> players;
    std::multimap<int, std::string> playersByRating;
    AVLRankTree rankingTree;

public:
    bool registerPlayer(const std::string& username);
    Player* findPlayer(const std::string& username);
    const Player* findPlayer(const std::string& username) const;
    bool applyRatingChange(const std::string& username, int ratingChange);
    bool recordWin(const std::string& username);
    bool recordLoss(const std::string& username);
    bool recordDraw(const std::string& username);
    bool removePlayer(const std::string& username);
    std::optional<std::string> findClosestRatedOpponent(
        const std::string& username) const;
    std::vector<std::string> getUsernames() const;
    int findOptimizedRank(const std::string& username) const;
    bool validateRankingIndex() const;
    int size() const;
};
