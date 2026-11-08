#include "Performance.h"
#include "HeapSort.h"
#include "MergeSort.h"
#include <chrono>

Performance sortPlayers(std::vector<Player>& players, Field field, bool useMergeSort) {
    Performance performance;
    if (useMergeSort) {
        // estimate the tmp players and their string contents
        size_t bytes = players.size() * sizeof(Player);
        for (const Player& p : players) {
            bytes += p.title.size() + p.username.size() + p.name.size() + p.country.size() + 4;
        }
        performance.extraMiB = bytes / (1024.0 * 1024.0);
    }

    auto start = std::chrono::steady_clock::now();
    if (useMergeSort) mergeSort(players, field);
    else heapSort(players, field);
    performance.sortMs = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
    return performance;
}

std::vector<Comparison> compareSorts(const std::vector<Player>& players, Field field) {
    std::vector<Comparison> comparisons;
    for (int scale = 1; scale <= 4; scale++) {
        int count = players.size() * scale / 4;
        if (count == 0 || (!comparisons.empty() && comparisons.back().count == count)) continue;

        Comparison comparison;
        comparison.count = count;
        // start each algorithm with same records
        {
            std::vector<Player> copy(players.begin(), players.begin() + count);
            comparison.heap = sortPlayers(copy, field, false);
        }
        {
            std::vector<Player> copy(players.begin(), players.begin() + count);
            comparison.merge = sortPlayers(copy, field, true);
        }
        comparisons.push_back(comparison);
    }
    return comparisons;
}
