#include "Leaderboard.h"
#include <cctype>
#include <cstdlib>

std::string lowercase(std::string text) {
    for (char& c : text) c = std::tolower(c);
    return text;
}

std::vector<Player> searchPlayers(const std::vector<Player>& players, Field field,
                                  const std::string& query) {
    std::vector<Player> result;
    std::string needle = lowercase(query);
    double value = std::atof(query.c_str());
    for (const Player& p : players) {
        // usernames match partially, numbers have to match exactly
        if ((field == Field::Username && lowercase(p.username).find(needle) != std::string::npos) ||
            (field == Field::WinRate && p.wonPercent == value) ||
            (field == Field::Rank && p.rank == value))
            result.push_back(p);
    }
    return result;
}

double percentile(int rank, int total) {
    return 100.0 * (total - rank) / (total - 1); // rank 1 is 100, last place is 0
}
