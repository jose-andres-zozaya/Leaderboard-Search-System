#pragma once
#include "Player.h"
#include <vector>

enum class Field { Username, WinRate, Rank };

bool comesBefore(const Player& a, const Player& b, Field field);
bool validSearchQuery(Field field, const std::string& query);

std::vector<Player> searchPlayers(const std::vector<Player>& players, Field field,
                                  const std::string& query);
double percentile(int rank, int total);
