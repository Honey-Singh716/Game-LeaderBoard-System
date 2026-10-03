#include "services/LeaderboardService.h"
#include "services/MatchService.h"
#include "services/MatchmakingService.h"
#include "services/RankingService.h"
#include "services/PlayerService.h"

#include <cassert>
#include <iostream>
#include <random>
#include <set>

void testPlayerService() {
    PlayerService players;
    assert(!players.registerPlayer(""));
    assert(!players.registerPlayer("ab"));
    assert(players.registerPlayer("Rahul_07"));
    assert(!players.registerPlayer("Rahul_07"));
    assert(players.findPlayer("Rahul_07") != nullptr);
    assert(players.findPlayer("Missing") == nullptr);

    const Player* player = players.findPlayer("Rahul_07");
    assert(player->username == "Rahul_07");
    assert(player->rating == 400);
    assert(player->wins == 0);
    assert(player->losses == 0);
    assert(player->draws == 0);
    assert(player->totalMatchesPlayed() == 0);
    assert(player->getWinRate() == 0.0);

    assert(players.removePlayer("Rahul_07"));
    assert(!players.removePlayer("Rahul_07"));
}

void testLeaderboardAndRanking() {
    PlayerService players;
    players.registerPlayer("One");
    players.registerPlayer("Two");
    players.registerPlayer("Three");
    for (int i = 0; i < 8; ++i) {
        players.recordWin("One");
    }
    for (int i = 0; i < 2; ++i) {
        players.recordLoss("One");
        players.recordWin("Two");
        players.recordLoss("Two");
        players.recordWin("Three");
        players.recordLoss("Three");
    }

    LeaderboardService leaderboard(players);
    RankingService ranking(players);
    assert(leaderboard.getTopK(0).empty());
    assert(leaderboard.getTopK(10).size() == 3);
    assert(leaderboard.getTopK(2).size() == 2);
    assert(leaderboard.getTopK(2).front().username == "One");
    assert(leaderboard.getTopK(2).front().getWinRate() == 0.8);
    assert(ranking.findRank("One") == 1);
    assert(ranking.findRank("Two") == 2);
    assert(ranking.findRank("Three") == 2);
    assert(ranking.findRank("Missing") == -1);
    assert(ranking.findNaiveRank("One") == ranking.findOptimizedRank("One"));
    assert(players.validateRankingIndex());
    players.removePlayer("One");
    assert(leaderboard.getTopK(10).size() == 2);
    assert(players.validateRankingIndex());
}

void testMatches() {
    PlayerService players;
    players.registerPlayer("One");
    assert(!MatchService(players, 7).playMatch("One").has_value());

    players.registerPlayer("Two");
    MatchService matches(players, 7);
    assert(!matches.playMatch("Missing").has_value());
    assert(matches.playMatch("One").has_value());

    const Match& match = matches.getHistory().front();
    assert(match.player1Username == "One");
    assert(match.player2Username == "Two");
    assert(match.result == MatchResult::PLAYER_1_WIN ||
           match.result == MatchResult::PLAYER_2_WIN ||
           match.result == MatchResult::DRAW);

    const Player* one = players.findPlayer("One");
    const Player* two = players.findPlayer("Two");
    assert(players.validateRankingIndex());
    assert(one->wins + one->losses + one->draws == 1);
    assert(two->wins + two->losses + two->draws == 1);
    assert(one->rating >= 0);
    assert(two->rating >= 0);

    if (match.result == MatchResult::PLAYER_1_WIN) {
        assert(one->rating == 420 && two->rating == 380);
        assert(one->getWinRate() == 1.0 && two->getWinRate() == 0.0);
    } else if (match.result == MatchResult::PLAYER_2_WIN) {
        assert(one->rating == 380 && two->rating == 420);
        assert(one->getWinRate() == 0.0 && two->getWinRate() == 1.0);
    } else {
        assert(one->rating == 400 && two->rating == 400);
        assert(one->getWinRate() == 0.0 && two->getWinRate() == 0.0);
    }
}

void testWinRateCalculations() {
    Player player{"WinRateUser"};
    player.wins = 5;
    player.losses = 3;
    player.draws = 2;
    assert(player.totalMatchesPlayed() == 10);
    assert(player.getWinRate() == 0.5);

    player.wins = 8;
    player.losses = 2;
    player.draws = 0;
    assert(player.getWinRate() == 0.8);
}

void testAVLTreeOperations() {
    for (const auto& rates : {
             std::vector<int>{3000, 2000, 1000},
             std::vector<int>{1000, 2000, 3000},
             std::vector<int>{3000, 1000, 2000},
             std::vector<int>{1000, 3000, 2000}}) {
        AVLRankTree tree;
        for (int rate : rates) {
            tree.insert(rate, std::to_string(rate));
        }
        assert(tree.size() == 3);
        assert(tree.validate());
        assert(tree.rankForWinRate(3000) == 1);
        assert(tree.rankForWinRate(2000) == 2);
        assert(tree.rankForWinRate(1000) == 3);
    }

    AVLRankTree tree;
    tree.insert(5000, "A");
    tree.insert(5000, "B");
    tree.insert(5000, "C");
    tree.insert(4000, "D");
    assert(tree.rankForWinRate(5000) == 1);
    assert(tree.rankForWinRate(4000) == 4);
    assert(tree.erase(5000, "B"));
    assert(tree.erase(5000, "A"));
    assert(tree.erase(5000, "C"));
    assert(tree.erase(4000, "D"));
    assert(tree.size() == 0);
    assert(tree.validate());
}

void testDynamicRankingAgainstNaive() {
    PlayerService players;
    constexpr int playerCount = 250;
    for (int i = 0; i < playerCount; ++i) {
        const std::string username = "User" + std::to_string(i);
        assert(players.registerPlayer(username));
        for (int win = 0; win < i % 9; ++win) {
            players.recordWin(username);
        }
        for (int loss = i % 9; loss < 12; ++loss) {
            players.recordLoss(username);
        }
        if (i % 4 == 0) {
            players.recordDraw(username);
        }
    }

    RankingService ranking(players);
    for (int i = 0; i < playerCount; ++i) {
        const std::string username = "User" + std::to_string(i);
        assert(ranking.findNaiveRank(username) ==
               ranking.findOptimizedRank(username));
    }
    assert(players.validateRankingIndex());

    assert(players.removePlayer("User100"));
    assert(ranking.findOptimizedRank("User100") == -1);
    assert(players.validateRankingIndex());
}

void testOptimizedMatchmaking() {
    PlayerService players;
    players.registerPlayer("Rahul");
    players.registerPlayer("Aman");
    players.registerPlayer("Karan");

    MatchmakingService matchmaking(players);
    auto sameRatingOpponent = matchmaking.findOpponent("Rahul");
    assert(sameRatingOpponent.has_value());
    assert(*sameRatingOpponent != "Rahul");

    MatchService matches(players, 11);
    assert(matches.playMatch("Rahul").has_value());

    auto updatedIndexOpponent = matchmaking.findOpponent("Karan");
    assert(updatedIndexOpponent.has_value());
    assert(*updatedIndexOpponent != "Karan");

    assert(players.removePlayer("Aman"));
    auto removedOpponent = matchmaking.findOpponent("Rahul");
    assert(removedOpponent.has_value());
    assert(*removedOpponent != "Aman");

    PlayerService soloPlayer;
    soloPlayer.registerPlayer("Solo");
    MatchmakingService soloMatchmaking(soloPlayer);
    assert(!soloMatchmaking.findOpponent("Solo").has_value());
}

void testRandomResultTypes() {
    PlayerService players;
    players.registerPlayer("One");
    players.registerPlayer("Two");
    MatchService matches(players, 42);
    std::set<MatchResult> results;
    for (int i = 0; i < 30; ++i) {
        auto match = matches.playMatch("One");
        assert(match.has_value());
        results.insert(match->result);
    }
    assert(results.size() == 3);
}

int main() {
    testPlayerService();
    testLeaderboardAndRanking();
    testMatches();
    testWinRateCalculations();
    testAVLTreeOperations();
    testDynamicRankingAgainstNaive();
    testOptimizedMatchmaking();
    testRandomResultTypes();
    std::cout << "All tests passed.\n";
}
