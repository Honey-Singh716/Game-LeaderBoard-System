#include "services/MatchService.h"

#include <algorithm>
#include <cstdlib>

MatchService::MatchService(PlayerService& playerService)
    : playerService(playerService),
      matchmakingService(playerService),
      generator(std::random_device{}()) {}

MatchService::MatchService(PlayerService& playerService, unsigned int seed)
    : playerService(playerService),
      matchmakingService(playerService),
      generator(seed) {}

MatchResult MatchService::generateResult() {
    std::uniform_int_distribution<int> distribution(0, 2);
    switch (distribution(generator)) {
    case 0:
        return MatchResult::PLAYER_1_WIN;
    case 1:
        return MatchResult::PLAYER_2_WIN;
    default:
        return MatchResult::DRAW;
    }
}

RatingChanges MatchService::getRatingChanges(MatchResult result) const {
    switch (result) {
    case MatchResult::PLAYER_1_WIN:
        return {20, -20};
    case MatchResult::PLAYER_2_WIN:
        return {-20, 20};
    case MatchResult::DRAW:
        return {0, 0};
    }
    return {0, 0};
}

std::optional<Match> MatchService::playMatch(
    const std::string& player1Username) {
    const Player* player1 = playerService.findPlayer(player1Username);
    if (player1 == nullptr) {
        return std::nullopt;
    }

    std::optional<std::string> player2Username =
        matchmakingService.findOpponent(player1Username);
    if (!player2Username.has_value()) {
        return std::nullopt;
    }

    const Player* player2 = playerService.findPlayer(*player2Username);
    MatchResult result = generateResult();
    RatingChanges changes = getRatingChanges(result);

    playerService.applyRatingChange(player1Username, changes.player1Rating);
    playerService.applyRatingChange(*player2Username, changes.player2Rating);

    switch (result) {
    case MatchResult::PLAYER_1_WIN:
        ++playerService.findPlayer(player1Username)->wins;
        ++playerService.findPlayer(*player2Username)->losses;
        break;
    case MatchResult::PLAYER_2_WIN:
        ++playerService.findPlayer(player1Username)->losses;
        ++playerService.findPlayer(*player2Username)->wins;
        break;
    case MatchResult::DRAW:
        ++playerService.findPlayer(player1Username)->draws;
        ++playerService.findPlayer(*player2Username)->draws;
        break;
    }

    Match match{
        nextMatchId++,
        player1->username,
        player2->username,
        result,
        changes.player1Rating,
        changes.player2Rating};
    history.push_back(match);
    return match;
}

const std::vector<Match>& MatchService::getHistory() const {
    return history;
}
