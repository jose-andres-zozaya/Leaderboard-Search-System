#include "DataLoader.h"
#include "Terminal.h"

int main(int argc, char* argv[]) {
    std::vector<Player> players = loadPlayers(argc > 1 ? argv[1] : DEFAULT_DATA_PATH);
    if (players.empty()) return 1;
    runTerminal(players);
}
