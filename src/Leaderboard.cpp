#include "Leaderboard.h"
#include <cctype>
#include <cmath>
#include <sstream>

bool comesBefore(const Player& a, const Player& b, Field field) {
    if (field == Field::Rank && a.rank != b.rank) return a.rank < b.rank;
    if (field == Field::WinRate && a.wonPercent != b.wonPercent) return a.wonPercent > b.wonPercent;
    return a.username < b.username;
}

std::string lowercase(std::string text) {
    for (char& c : text) c = std::tolower(static_cast<unsigned char>(c));
    return text;
}

bool validSearchQuery(Field field, const std::string& query) {
    if (field == Field::Username || query.empty()) return true;

    double value;
    std::istringstream input(query);
    if (!(input >> value) || !(input >> std::ws).eof() || !std::isfinite(value)) return false;
    if (field == Field::WinRate) return value >= 0 && value <= 100;
    return value >= 1 && std::floor(value) == value;
}

std::vector<Player> searchPlayers(const std::vector<Player>& players, Field field,
                                  const std::string& query) {
    std::vector<Player> result;
    if (!validSearchQuery(field, query)) return result;
    if (query.empty()) return players;
    std::string needle = lowercase(query);
    double value = 0;
    if (field != Field::Username) std::istringstream(query) >> value;
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
    if (total <= 1) return 100.0;
    return 100.0 * (total - rank) / (total - 1); // rank 1 is 100, last place is 0
}
