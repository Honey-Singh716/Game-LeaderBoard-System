#include "ui/ConsoleUI.h"

int main() {
    PlayerService playerService;
    playerService.registerPlayer("ShadowX");
    playerService.registerPlayer("Blaze");
    playerService.registerPlayer("Nova");
    playerService.registerPlayer("Ghost");
    playerService.registerPlayer("Dragon");

    LeaderboardService leaderboardService(playerService);
    RankingService rankingService(playerService);
    MatchService matchService(playerService);
    ConsoleUI ui(playerService, leaderboardService, rankingService, matchService);
    ui.run();
}
