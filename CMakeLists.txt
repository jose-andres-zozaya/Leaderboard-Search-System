cmake_minimum_required(VERSION 3.16)
project(LeaderboardSearch LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(leaderboard ${SOURCES})
target_include_directories(leaderboard PRIVATE src ${CURSES_INCLUDE_DIRS})
target_link_libraries(leaderboard PRIVATE ${CURSES_LIBRARIES})
target_compile_definitions(leaderboard PRIVATE DEFAULT_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/players_blitz.csv")
