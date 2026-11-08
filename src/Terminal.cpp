#include "Terminal.h"
#include "Performance.h"
#include <algorithm>
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
    Performance performance;
    std::vector<Comparison> comparisons;
    int offset = 0; // first result shown on screen
    bool searched = false;
    bool useMergeSort = false;
    bool showComparison = false;
    bool invalidQuery = false;

    while (true) {
        int page = std::max(0, LINES - 4);
        const std::vector<Player>& visible = searched ? result : players;
        int count = visible.size();
        erase();
        // highlight the field being searched
        for (int i = 0; i < 3; i++) {
            attrset(static_cast<int>(field) == i ? A_BOLD | COLOR_PAIR(1) : A_DIM);
            mvprintw(0, i * 13, static_cast<int>(field) == i ? "[%s]" : "%s", fields[i]);
        }
        attrset(COLOR_PAIR(1));
        mvprintw(0, 35, ">");
        attrset(A_DIM);
        if (showComparison) {
            mvprintw(1, 0, "PLAYERS      HEAP ms     MERGE ms    HEAP MiB   MERGE MiB");
        } else {
            mvprintw(1, 0, "   RANK  USERNAME                    WIN %%  PERCENTILE");
        }
        attrset(A_NORMAL);
        if (showComparison) {
            for (int i = 0; i < page && i < static_cast<int>(comparisons.size()); i++) {
                const Comparison& c = comparisons[i];
                mvprintw(2 + i, 0, "%7d %12.2f %12.2f %11.2f %11.2f", c.count,
                         c.heap.sortMs, c.merge.sortMs, c.heap.extraMiB, c.merge.extraMiB);
            }
        }
        for (int i = 0; !showComparison && i < page && offset + i < count; i++) {
            const Player& p = visible[offset + i];
            mvprintw(2 + i, 0, "%7d  %-25.25s%7.1f%11.1f", p.rank, p.username.c_str(), p.wonPercent,
                     percentile(p.rank, players.size()));
        }
        if (!showComparison && searched && count == 0) mvprintw(2, 0, "No results found");
        attrset(COLOR_PAIR(1));
        if (showComparison) {
            mvprintw(LINES - 2, 0, "Extra MiB estimated: merge O(n), heap O(1); both O(n log n) time");
        } else {
            mvprintw(LINES - 2, 0, "%s", useMergeSort ? "Merge sort" : "Heap sort");
            if (searched) printw("   %.2f ms   %.2f extra MiB (est.)   %d results",
                                 performance.sortMs, performance.extraMiB, count);
            else printw("   %d players", count);
            if (invalidQuery) {
                move(LINES - 2, 0);
                clrtoeol();
                printw("%s", field == Field::WinRate ? "Enter a win rate from 0 to 100" : "Enter a whole rank of 1 or higher");
            }
        }
        attrset(A_DIM);
        mvprintw(LINES - 1, 0, "Tab:field F2:sort F3:compare/back Enter:search Arrows/Pg:scroll ^Q:quit");
        attrset(A_NORMAL);
        mvprintw(0, 37, "%s", query.c_str());
        refresh();

        int key = getch();
        if (key == 17) break; // ctrl+q quits
        if (key == '\t') {
            field = static_cast<Field>((static_cast<int>(field) + 1) % 3);
            query.clear();
            result.clear();
            searched = false;
            showComparison = false;
            invalidQuery = false;
            offset = 0;
        } else if (key == KEY_F(2)) {
            useMergeSort = !useMergeSort;
            result.clear();
            searched = false;
            offset = 0;
            showComparison = false;
            invalidQuery = false;
        } else if (key == KEY_F(3)) {
            if (!showComparison) {
                mvprintw(LINES - 2, 0, "Comparing...");
                clrtoeol();
                refresh();
                comparisons = compareSorts(players, field);
            }
            showComparison = !showComparison;
        } else if (key == '\n') {
            invalidQuery = !validSearchQuery(field, query);
            if (invalidQuery) continue;
            // sort a fresh copy each search and time how long it takes
            std::vector<Player> sorted = players;
            performance = sortPlayers(sorted, field, useMergeSort);
            result = searchPlayers(sorted, field, query);
            searched = true;
            showComparison = false;
            offset = 0; // jump back to the top
        } else if (key == KEY_UP) offset = std::max(0, offset - 1);
        else if (key == KEY_DOWN) offset = std::min(offset + 1, std::max(0, count - page));
        else if (key == KEY_PPAGE) offset = std::max(0, offset - page);
        else if (key == KEY_NPAGE) offset = std::min(offset + page, std::max(0, count - page));
        else if ((key == KEY_BACKSPACE || key == 127) && !query.empty()) {
            query.pop_back();
            invalidQuery = false;
        } else if (key >= 32 && key <= 126) {
            query += static_cast<char>(key);
            invalidQuery = false;
        }
    }
    endwin();
}
