#pragma once

#include "services/LeaderboardService.h"
#include "services/MatchService.h"
#include "services/RankingService.h"
#include "services/PlayerService.h"

class ConsoleUI {
private:
    PlayerService& playerService;
    LeaderboardService& leaderboardService;
    RankingService& rankingService;
    MatchService& matchService;

    void printMenu() const;
    void registerPlayer();
    void showPlayer();
    void showLeaderboard();
    void showRank();
    void playMatch();
    void showMatchHistory() const;
    void removePlayer();

public:
    ConsoleUI(
        PlayerService& playerService,
        LeaderboardService& leaderboardService,
        RankingService& rankingService,
        MatchService& matchService);
    void run();
};
