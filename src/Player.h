#pragma once
#include <string>

struct Player {
    std::string title;
    std::string username;
    std::string name;
    std::string country;

    int rating = 0;
    int games = 0;
    int won = 0;
    int draw = 0;
    int lost = 0;
    int rank = 0;

    double wonPercent = 0.0;
    double drawPercent = 0.0;
    double lostPercent = 0.0;



};