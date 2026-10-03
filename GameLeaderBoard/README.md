# 🎮 Game Leaderboard & Matchmaking System

> **C++ DSA-focused competitive game backend simulation built around real algorithmic problems, not arbitrary data-structure additions.**

## What it demonstrates

- Username-only player identity
- Rating-based automatic matchmaking
- Automatic WIN / LOSS / DRAW generation
- Win Rate = `wins / (wins + losses + draws)`
- Win-Rate leaderboard and competition ranking
- Top-K using a size-K min heap
- Custom AVL tree + subtree size for dynamic `O(log n)` ranking
- Naive `O(n)` vs optimized ranking benchmark
- Randomized correctness testing
- CMake-based build

## DSA Design

| Problem | Structure | Complexity |
|---|---|---:|
| Player lookup | `unordered_map<string, Player>` | Avg. `O(1)` |
| Closest-rated opponent | `multimap<int, string>` + `lower_bound()` | `O(log n)` |
| Top-K | Size-K min heap | `O(n log k)` |
| Naive rank | Full scan | `O(n)` |
| Optimized dynamic rank | Custom AVL + subtree size | `O(log n)` |

The `unordered_map` is the **source of truth**. The multimap, AVL tree and heap are derived/query structures.

## Rating vs Win Rate

**Rating** is used for matchmaking:

```text
Initial = 400
Win  → +20
Loss → -20
Draw → 0
```

**Win Rate** is used for performance/ranking:

```text
Win Rate = wins / (wins + losses + draws)
```

No matches → `0%`.

## Matchmaking

The user enters only their username. The system automatically searches for the closest current rating.

```text
Rahul = 400
Aman  = 390  → diff 10
Karan = 410  → diff 10
Ravi  = 520  → diff 120
```

No opponent or result is manually selected.

## AVL Ranking

The baseline ranking scans all players:

```text
O(n)
```

V3 adds a custom AVL tree storing:

```text
Win Rate Key
Username
Height
Subtree Size
```

Rank query:

```text
O(log n)
```

Competition ranking is preserved, including ties.

Win Rate is converted to a deterministic integer key:

```cpp
winRateKey =
    static_cast<int>(std::lround(getWinRate() * 10000.0));
```

Examples:

```text
50%   → 5000
72.5% → 7250
80%   → 8000
```

## Benchmark

The benchmark performs **1,000 rank queries** per implementation.

| Players | Naive | AVL | Approx. Speedup |
|---:|---:|---:|---:|
| 1,000 | 676,877 µs | 210 µs | ~3,223× |
| 10,000 | 7,251,198 µs | 274 µs | ~26,464× |
| 100,000 | 106,131,741 µs | 442 µs | ~240,117× |

The important theoretical result is:

```text
Naive → O(n)
AVL   → O(log n)
```

> Benchmark values are environment-dependent and should not be treated as universal hardware guarantees.

## Testing

Covers player/match behavior plus AVL correctness:

- Registration, lookup and removal
- Win / Loss / Draw
- Win Rate edge cases
- Closest-rating matchmaking
- Self-match / no-opponent cases
- AVL rotations and deletion cases
- Duplicate Win Rates
- Competition ranking
- Subtree-size and balance invariants
- Randomized **Naive Rank == AVL Rank** verification

## Architecture

```text
ConsoleUI
    ↓
Services
├── PlayerService
├── MatchService
├── MatchmakingService
├── LeaderboardService
└── RankingService
    ↓
Algorithms / Models
├── AVLRankTree
├── MinHeap
├── Player
├── Match
└── MatchResult
    ↓
In-memory state
```

## Build

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build -C Debug --output-on-failure
.\build\Debug\game_leaderboard.exe
```

## Current Scope

The project is intentionally **in-memory and DSA-first**.

Not included yet:

```text
PostgreSQL
Redis
REST API
React
Microservices
Distributed matchmaking
```

These should be introduced only when a real scalability or product requirement justifies them.

## Interview Pitch

> I built a C++ competitive-game backend using different DSA structures for different workloads: an unordered_map for average O(1) player lookup, a multimap for O(log n) closest-rating matchmaking, a size-K min heap for O(n log k) Top-K retrieval, and a custom AVL tree with subtree sizes to reduce dynamic rank queries from O(n) to O(log n). I also kept the naive algorithm as a correctness baseline and benchmarked both implementations.
