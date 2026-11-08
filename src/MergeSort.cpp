#include "MergeSort.h"

static void merge(std::vector<Player>& players, std::vector<Player>& temp,
                  int left, int middle, int right, Field field) {
    int i = left;
    int j = middle + 1;
    int k = left;

    // take the next player from either half
    while (i <= middle && j <= right) {
        if (comesBefore(players[j], players[i], field)) {
            temp[k++] = players[j++];
        } else {
            temp[k++] = players[i++];
        }
    }
    // copy any players left over
    while (i <= middle) temp[k++] = players[i++];
    while (j <= right) temp[k++] = players[j++];

    for (int index = left; index <= right; index++) {
        players[index] = temp[index];
    }
}

static void mergeSortRange(std::vector<Player>& players, std::vector<Player>& temp,
                           int left, int right, Field field) {
    if (left >= right) return;

    int middle = left + (right - left) / 2;
    // sort both halves before merging
    mergeSortRange(players, temp, left, middle, field);
    mergeSortRange(players, temp, middle + 1, right, field);
    merge(players, temp, left, middle, right, field);
}

void mergeSort(std::vector<Player>& players, Field field) {
    // reuse one temporary vector for all merges
    std::vector<Player> temp(players.size());
    mergeSortRange(players, temp, 0, static_cast<int>(players.size()) - 1, field);
}
