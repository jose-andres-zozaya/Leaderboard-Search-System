#include "Terminal.h"
#include "HeapSort.h"
#include <algorithm>
#include <chrono>
#include <curses.h>

void runTerminal(const std::vector<Player>& players) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_CYAN, -1); // cyan for highlighted text

    const char* fields[] = {"Username", "Win rate", "Rank"};
    Field field = Field::Username;
    std::string query;
    std::vector<Player> result;
    double sortMs = 0;
    int offset = 0; // first result shown on screen
    bool searched = false;

    while (true) {
        int page = LINES - 3;
        int count = result.size();
        erase();
        // highlight the field being searched
        for (int i = 0; i < 3; i++) {
            attrset(static_cast<int>(field) == i ? A_BOLD | COLOR_PAIR(1) : A_DIM);
            mvprintw(0, i * 11, "%s", fields[i]);
        }
        attrset(COLOR_PAIR(1));
        mvprintw(0, 29, ">");
        attrset(A_DIM);
        mvprintw(1, 0, "   RANK  USERNAME                    WIN %%  PERCENTILE");
        attrset(A_NORMAL);
        for (int i = 0; i < page && offset + i < count; i++) {
            const Player& p = result[offset + i];
            mvprintw(2 + i, 0, "%7d  %-25.25s%7.1f%11.1f", p.rank, p.username.c_str(), p.wonPercent,
                     percentile(p.rank, players.size()));
        }
        if (searched && count == 0) mvprintw(2, 0, "No results found");
        attrset(COLOR_PAIR(1));
        mvprintw(LINES - 1, 0, "Heap sort");
        if (searched) printw("   %.2f ms   %d results", sortMs, count);
        attrset(A_NORMAL);
        mvprintw(0, 31, "%s", query.c_str());
        refresh();

        int key = getch();
        if (key == 17) break; // ctrl+q quits
        if (key == '\t') {
            field = static_cast<Field>((static_cast<int>(field) + 1) % 3);
            query.clear();
            result.clear();
            searched = false;
        } else if (key == '\n') {
            // sort a fresh copy each search and time how long it takes
            std::vector<Player> sorted = players;
            auto start = std::chrono::steady_clock::now();
            heapSort(sorted, field);
            sortMs = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
            result = searchPlayers(sorted, field, query);
            searched = true;
            offset = 0; // jump back to the top
        } else if (key == KEY_UP) offset = std::max(0, offset - 1);
        else if (key == KEY_DOWN) offset = std::min(offset + 1, std::max(0, count - page));
        else if (key == KEY_PPAGE) offset = std::max(0, offset - page);
        else if (key == KEY_NPAGE) offset = std::min(offset + page, std::max(0, count - page));
        else if ((key == KEY_BACKSPACE || key == 127) && !query.empty()) query.pop_back();
        else if (key >= 32 && key <= 126) query += static_cast<char>(key);
    }
    endwin();
}
