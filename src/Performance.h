#pragma once
#include "Leaderboard.h"

struct Performance {
    double sortMs = 0;
    double extraMiB = 0;
};

struct Comparison {
    int count = 0;
    Performance heap;
    Performance merge;
};

Performance sortPlayers(std::vector<Player>& players, Field field, bool useMergeSort);
std::vector<Comparison> compareSorts(const std::vector<Player>& players, Field field);
