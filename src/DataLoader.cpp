#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<Player> loadPlayers(const std::string &path) {
    std::vector<Player> players;
    players.reserve(200000); // allocates room for all 200,000 players up front so the vector doesn't constantly reallocate as it grows
    int skipped = 0;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open " << path << std::endl;
        return players;
    }

    std::string line;
    std::getline(file, line); // header row

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string f[12];
        for (int i = 0; i < 12; i++) {
            std::getline(ss, f[i], ',');
        }

        try {
            Player p;
            p.title = f[0];
            p.username = f[1];
            p.name = f[2];
            p.rating = std::stoi(f[3]);
            p.country = f[4];
            p.games = std::stoi(f[5]);
            p.won = std::stoi(f[6]);
            p.draw = std::stoi(f[7]);
            p.lost = std::stoi(f[8]);
            p.wonPercent = std::stod(f[9]);
            p.drawPercent = std::stod(f[10]);
            p.lostPercent = std::stod(f[11]);
            players.push_back(p);
        } catch (...)  {
            skipped++;
        }
    }

    std::cout << "Loaded " << players.size() << " player, skipped " << skipped << std::endl;
    assignRanks(players);
    return players;
}

void assignRanks(std::vector<Player> &players) {
    auto higherRating = [](const Player& a, const Player& b) {
        return a.rating > b.rating;
    };
    if (!std::is_sorted(players.begin(), players.end(), higherRating)) {
        std::stable_sort(players.begin(), players.end(), higherRating);
    }
    for (size_t i = 0; i < players.size(); i++) {
        if (i > 0 && players[i].rating == players[i-1].rating) {
            players[i].rank = players[i-1].rank;
        } else {
            players[i].rank = static_cast<int>(i) + 1;
        }
    }
}
