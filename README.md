# Leaderboard Search COP3530 Project 2

A terminal app for searching 200,000 Chess.com blitz players and comparing merge sort with heap sort.

## Requirements

CMake 3.16+ and C/C++ compilers with C++17 support.

- Windows: Visual Studio 2022 with Desktop development with C++, CMake 3.21+, and Git available on PATH. CMake downloads and builds PDCurses automatically
- Linux/macOS: ncurses development headers. On Ubuntu, get the dependencies:

```sh
sudo apt install build-essential cmake libncurses-dev
```

## Build and run

From the repository folder, on Linux/macOS:

```sh
cmake -S . -B build
cmake --build build
```

On Windows PowerShell:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

Run in terminal:

- Windows (Visual Studio): `.\build\Release\leaderboard.exe`
- Linux/macOS: `./build/leaderboard`

## Controls

Tab selects the search field. Enter searches. F2 switches sorting algorithms. F3 opens or closes the performance comparison. Use arrow keys or Page Up/Page Down to scroll. Ctrl+Q to quit.
