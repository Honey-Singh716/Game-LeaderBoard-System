#include "services/PlayerService.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace {
bool isValidUsername(const std::string& username) {
    if (username.size() < 3 || username.size() > 20) {
        return false;
    }

    for (char character : username) {
        if (!std::isalnum(static_cast<unsigned char>(character)) &&
            character != '_' && character != '-') {
            return false;
        }
    }
    return true;
}
}

bool PlayerService::registerPlayer(const std::string& username) {
    if (!isValidUsername(username) ||
        players.find(username) != players.end()) {
        return false;
    }
    players.emplace(username, Player{username});
    playersByRating.emplace(400, username);
    return true;
}

Player* PlayerService::findPlayer(const std::string& username) {
    auto it = players.find(username);
    return it == players.end() ? nullptr : &it->second;
}

const Player* PlayerService::findPlayer(const std::string& username) const {
    auto it = players.find(username);
    return it == players.end() ? nullptr : &it->second;
}

bool PlayerService::applyRatingChange(
    const std::string& username,
    int ratingChange) {
    Player* player = findPlayer(username);
    if (player == nullptr) {
        return false;
    }

    auto range = playersByRating.equal_range(player->rating);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == username) {
            playersByRating.erase(it);
            break;
        }
    }

    player->rating = std::max(0, player->rating + ratingChange);
    playersByRating.emplace(player->rating, username);
    return true;
}

bool PlayerService::removePlayer(const std::string& username) {
    auto player = players.find(username);
    if (player == players.end()) {
        return false;
    }

    auto range = playersByRating.equal_range(player->second.rating);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == username) {
            playersByRating.erase(it);
            break;
        }
    }
    players.erase(player);
    return true;
}

std::optional<std::string> PlayerService::findClosestRatedOpponent(
    const std::string& username) const {
    const Player* player = findPlayer(username);
    if (player == nullptr || players.size() < 2) {
        return std::nullopt;
    }

    const auto lower = playersByRating.lower_bound(player->rating);
    std::optional<std::string> bestUsername;
    int bestDifference = 0;

    auto consider = [&](auto it) {
        if (it == playersByRating.end() || it->second == username) {
            return;
        }

        int difference = std::abs(player->rating - it->first);
        if (!bestUsername.has_value() || difference < bestDifference) {
            bestUsername = it->second;
            bestDifference = difference;
        }
    };

    auto right = lower;
    if (right != playersByRating.end() && right->second == username) {
        ++right;
    }
    consider(right);

    auto left = lower;
    if (left != playersByRating.begin()) {
        --left;
        if (left->second == username && left != playersByRating.begin()) {
            --left;
        }
        consider(left);
    }

    return bestUsername;
}

std::vector<std::string> PlayerService::getUsernames() const {
    std::vector<std::string> usernames;
    usernames.reserve(players.size());
    for (const auto& [username, unusedPlayer] : players) {
        usernames.push_back(username);
    }
    return usernames;
}

int PlayerService::size() const {
    return static_cast<int>(players.size());
}
