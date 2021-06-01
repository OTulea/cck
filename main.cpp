#include <vector>
#include <iostream>
#include "world.hpp"
#include "curses.h"

#define COLOR_GREY COLOR_CYAN

int main()
{
    // CURSES SETUP
    initscr(); // pdcurses starts in cbreak by default
    noecho();
    curs_set(0);
    start_color();
    init_color(COLOR_GREY, 400, 400, 400);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_GREY, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    // GAME SETUP
    World game(COLS * LINES, COLS); //deal with resizing eventually
    char input;
    while (true)
    {
        input = getch();
        switch (input)
        {
        case ('w'):
        case ('a'):
        case ('s'):
        case ('d'):
            game.interact(input);
            move(0, 0);
            refresh();
            break;
        }
    }
    endwin();
};