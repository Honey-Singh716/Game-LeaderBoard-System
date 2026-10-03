#include "ui/ConsoleUI.h"

#include "models/MatchResult.h"

#include <iomanip>
#include <iostream>

namespace {
std::string matchResultDescription(const Match& match) {
    switch (match.result) {
    case MatchResult::PLAYER_1_WIN:
        return match.player1Username + " WON";
    case MatchResult::PLAYER_2_WIN:
        return match.player2Username + " WON";
    case MatchResult::DRAW:
        return "MATCH DRAW";
    }
    return "UNKNOWN";
}
}

ConsoleUI::ConsoleUI(
    PlayerService& playerService,
    LeaderboardService& leaderboardService,
    RankingService& rankingService,
    MatchService& matchService)
    : playerService(playerService),
      leaderboardService(leaderboardService),
      rankingService(rankingService),
      matchService(matchService) {}

void ConsoleUI::printMenu() const {
    std::cout << "\n======= GAME LEADERBOARD SYSTEM =======\n"
              << "1. Register Player\n"
              << "2. Find Player\n"
              << "3. View Player Profile\n"
              << "4. View Leaderboard\n"
              << "5. Find My Rank\n"
              << "6. Play Match\n"
              << "7. View Match History\n"
              << "8. Remove Player\n"
              << "0. Exit\n"
              << "Enter choice: ";
}

void ConsoleUI::registerPlayer() {
    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;
    if (playerService.registerPlayer(username)) {
        std::cout << "Player registered successfully.\n"
                  << "Username: " << username << '\n'
                  << "Rating: 400\n";
    } else {
        std::cout << "Invalid or duplicate username.\n";
    }
}

void ConsoleUI::showPlayer() {
    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;
    const Player* player = playerService.findPlayer(username);
    if (player == nullptr) {
        std::cout << "Player not found\n";
        return;
    }
    std::cout << "Username: " << player->username << '\n'
              << "Rating: " << player->rating << '\n'
              << "Wins: " << player->wins << '\n'
              << "Losses: " << player->losses << '\n'
              << "Draws: " << player->draws << '\n'
              << "Total Matches: " << player->totalMatchesPlayed() << '\n'
              << "Win Rate: " << std::fixed << std::setprecision(2)
              << player->getWinRate() * 100.0 << "%\n"
              << std::defaultfloat;
}

void ConsoleUI::showLeaderboard() {
    int k;
    std::cout << "Enter leaderboard size: ";
    std::cin >> k;
    std::cout << "\n============= LEADERBOARD =============\n";
    int rank = 1;
    for (const Player& player : leaderboardService.getTopK(k)) {
        std::cout << rank++ << ". " << player.username
                  << " | Win Rate: " << std::fixed << std::setprecision(2)
                  << player.getWinRate() * 100.0 << "%"
                  << " | Rating: " << player.rating << '\n'
                  << std::defaultfloat;
    }
    std::cout << "========================================\n";
}

void ConsoleUI::showRank() {
    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;
    const Player* player = playerService.findPlayer(username);
    int rank = rankingService.findRank(username);
    if (player == nullptr) {
        std::cout << "Player not found\n";
    } else {
        std::cout << player->username << " has rank " << rank << '\n';
    }
}

void ConsoleUI::playMatch() {
    std::string username;
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::optional<Match> match = matchService.playMatch(username);
    if (!match.has_value()) {
        std::cout << "Unable to start match. Check the player and available opponents.\n";
        return;
    }
    std::cout << "Finding opponent...\n"
              << "Opponent found: " << match->player2Username << '\n'
              << match->player1Username << " VS "
              << match->player2Username << '\n'
              << "Match result: " << matchResultDescription(*match) << '\n';
}

void ConsoleUI::showMatchHistory() const {
    std::cout << "\n============= MATCH HISTORY =============\n";
    if (matchService.getHistory().empty()) {
        std::cout << "No completed matches.\n";
    }
    for (auto it = matchService.getHistory().rbegin();
         it != matchService.getHistory().rend();
         ++it) {
        std::cout << "Match #" << it->matchId << '\n'
                  << it->player1Username << " vs " << it->player2Username << '\n'
                  << "Result: " << matchResultDescription(*it) << '\n'
                  << "Rating changes: " << it->player1RatingChange << " / "
                  << it->player2RatingChange << "\n\n";
    }
    std::cout << "==========================================\n";
}

void ConsoleUI::removePlayer() {
    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << (playerService.removePlayer(username)
                      ? "Player removed successfully.\n"
                      : "Player not found\n");
}

void ConsoleUI::run() {
    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice) || choice == 0) {
            return;
        }
        switch (choice) {
        case 1:
            registerPlayer();
            break;
        case 2:
        case 3:
            showPlayer();
            break;
        case 4:
            showLeaderboard();
            break;
        case 5:
            showRank();
            break;
        case 6:
            playMatch();
            break;
        case 7:
            showMatchHistory();
            break;
        case 8:
            removePlayer();
            break;
        default:
            std::cout << "Invalid choice.\n";
        }
    }
}
