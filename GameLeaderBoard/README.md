# Game Leaderboard System

This C++ project models a small competitive-game backend. It preserves the V1
leaderboard while adding an in-memory V2 match system. Players are identified
by unique usernames; there are no numeric player IDs.

## Features

- Register, find, view, and remove players
- New players start with rating `400`
- Calculated Win Rate from wins, losses, and draws
- Top-K leaderboard and competition ranking by Win Rate
- Automatic nearest-rating opponent selection
- Random win, loss, or draw generation
- Rating and win/loss/draw updates
- In-memory match history
- Console UI separated from domain models and services

## Architecture and DSA

- `PlayerService` owns player state in an `unordered_map` for average `O(1)`
  lookup, registration, and removal.
- `LeaderboardService` scans players with a Win Rate min heap of size `K`,
  giving `O(n log K)` Top-K retrieval and `O(K)` heap space.
- `RankingService` uses competition ranking:
  `1 + number of players with a strictly higher Win Rate`, in `O(n)`.
`MatchmakingService` now uses a synchronized `std::multimap<int, std::string>`
rating index for closest-opponent lookup in `O(log n)`. The
`unordered_map<std::string, Player>` remains the source of truth. The index is
updated on registration, removal, and match-driven rating changes.

Win Rate and rating are separate:

- Win Rate is calculated as `wins / (wins + losses + draws)`.
- A player with no matches has a `0%` Win Rate.
- Match win: rating `+20`
- Match loss: rating `-20`
- Match draw: rating unchanged

The match service centralizes rating and statistics updates. The leaderboard and
rank use Win Rate; rating is used only for matchmaking.

## Build and run

From `GameLeaderBoard/`:

```text
cmake -S . -B build
cmake --build build
ctest --test-dir build -C Debug --output-on-failure
build\Debug\game_leaderboard.exe
```

On single-configuration generators, the executable may be directly under
`build/`. The project can also be compiled with `g++` using the sources listed
in `CMakeLists.txt`.

## Progress

- V1 Basic Leaderboard: completed
- V2 Match System: completed
- Future: event-based scoring, matchmaking performance measurement, persistence,
  and API boundaries only when justified by requirements
