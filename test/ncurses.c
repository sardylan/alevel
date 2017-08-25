#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

int main(int argc, char **arv) {
    WINDOW *window;
    struct timespec sleep_time;
    int y;
    int x;
    short c;

    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 10000;

    c = 0;

    window = initscr();
    if (window == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    start_color();
    if (!has_colors() || COLOR_PAIRS < 13) {
        delwin(window);
        endwin();
        refresh();
        fprintf(stderr, "Ncurses doesn't have colors.\n");
        exit(EXIT_FAILURE);
    }

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_WHITE);
    init_pair(8, COLOR_WHITE, COLOR_RED);
    init_pair(9, COLOR_BLACK, COLOR_GREEN);
    init_pair(10, COLOR_BLUE, COLOR_YELLOW);
    init_pair(11, COLOR_WHITE, COLOR_BLUE);
    init_pair(12, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(13, COLOR_BLACK, COLOR_CYAN);

    for (y = 0; y < 30; y++) {
        for (x = 0; x < 80; x++) {
            color_set(c, NULL);
            mvaddstr(y, x, "#");
            refresh();

            c++;
            c %= 13;
            nanosleep(&sleep_time, NULL);
        }
    }

    sleep(5);

    delwin(window);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}