#pragma once

#include <queue>
#include <string>
#include <vector>

struct HeapEntry {
    std::string username;
    double winRate;
};

struct CompareWinRate {
    bool operator()(const HeapEntry& a, const HeapEntry& b) const {
        if (a.winRate != b.winRate) {
            return a.winRate > b.winRate;
        }
        return a.username > b.username;
    }
};

using WinRateMinHeap = std::priority_queue<
    HeapEntry,
    std::vector<HeapEntry>,
    CompareWinRate>;
