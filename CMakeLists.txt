cmake_minimum_required(VERSION 3.16)
project(LeaderboardSearch LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(leaderboard ${SOURCES})
target_include_directories(leaderboard PRIVATE src )