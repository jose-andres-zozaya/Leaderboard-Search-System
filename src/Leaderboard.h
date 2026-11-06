#pragma once
#include "Player.h"
#include <vector>

enum class Field { Username, WinRate, Rank };

std::vector<Player> searchPlayers(const std::vector<Player>& players, Field field,
                                  const std::string& query);
