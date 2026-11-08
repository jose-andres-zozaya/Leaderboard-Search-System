#include "HeapSort.h"
#include <utility>

void siftDown(std::vector<Player>& players, int root, int size, Field field) {
    while (2 * root + 1 < size) {
        int child = 2 * root + 1;
        if (child + 1 < size && comesBefore(players[child], players[child + 1], field)) child++;
        if (!comesBefore(players[root], players[child], field)) return;
        std::swap(players[root], players[child]);
        root = child;
    }
}

void heapSort(std::vector<Player>& players, Field field) {
    int n = players.size();
    for (int i = n / 2 - 1; i >= 0; i--) siftDown(players, i, n, field); // turn the list into a heap
    for (int end = n - 1; end > 0; end--) {
        std::swap(players[0], players[end]); // move the top of the heap to the end
        siftDown(players, 0, end, field);
    }
}
