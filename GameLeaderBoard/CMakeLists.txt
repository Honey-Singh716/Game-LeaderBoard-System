cmake_minimum_required(VERSION 3.16)
project(GameLeaderBoard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GAME_SOURCES
    src/algorithms/AVLRankTree.cpp
    src/models/Match.cpp
    src/models/MatchResult.cpp
    src/models/Player.cpp
    src/services/LeaderboardService.cpp
    src/services/MatchService.cpp
    src/services/MatchmakingService.cpp
    src/services/PlayerService.cpp
    src/services/RankingService.cpp
)

add_executable(game_leaderboard
    ${GAME_SOURCES}
    src/main.cpp
    src/ui/ConsoleUI.cpp
)
target_include_directories(game_leaderboard PRIVATE include)

add_executable(game_leaderboard_tests
    ${GAME_SOURCES}
    tests/test_main.cpp
)
target_include_directories(game_leaderboard_tests PRIVATE include)

add_executable(ranking_benchmark
    ${GAME_SOURCES}
    benchmarks/ranking_benchmark.cpp
)
target_include_directories(ranking_benchmark PRIVATE include)

enable_testing()
add_test(NAME game_leaderboard_tests COMMAND game_leaderboard_tests)
