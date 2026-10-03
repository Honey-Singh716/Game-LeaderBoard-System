#include "services/RankingService.h"

#include <chrono>
#include <iostream>
#include <string>

namespace {
void populate(PlayerService& players, int count) {
    for (int i = 0; i < count; ++i) {
        const std::string username = "Player" + std::to_string(i);
        players.registerPlayer(username);
        for (int match = 0; match < i % 10; ++match) {
            players.recordWin(username);
        }
        for (int match = i % 10; match < 10; ++match) {
            players.recordLoss(username);
        }
    }
}

void runBenchmark(int count) {
    PlayerService players;
    populate(players, count);
    RankingService ranking(players);
    const std::string target = "Player" + std::to_string(count / 2);
    constexpr int iterations = 1000;

    auto measure = [&](auto query) {
        const auto start = std::chrono::steady_clock::now();
        volatile int result = 0;
        for (int i = 0; i < iterations; ++i) {
            result += query(target);
        }
        const auto end = std::chrono::steady_clock::now();
        (void)result;
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   end - start)
            .count();
    };

    std::cout << count << " players: naive "
              << measure([&](const std::string& name) {
                     return ranking.findNaiveRank(name);
                 })
              << " us, AVL "
              << measure([&](const std::string& name) {
                     return ranking.findOptimizedRank(name);
                 })
              << " us\n";
}
}

int main() {
    runBenchmark(1000);
    runBenchmark(10000);
    runBenchmark(100000);
}
