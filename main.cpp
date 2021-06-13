#include "curses.h"
#include"world.hpp"

#define COLOR_GREY 65

int main()
{
    // CURSES SETUP
    initscr(); // pdcurses starts in cbreak by default
    noecho();
    curs_set(0);
    start_color(); // recompile curses with wide chars for more colours
    init_color(COLOR_GREY, 400, 400, 400);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_GREY, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    // GAME SETUP
    // WINDOWS TERMINAL is 120 by 30 base;
    World game(COLS / 2, LINES - 1); //deal with resizing eventually
    while (true)
    {
        char input = getch(); //this blocks until input received
        switch (input)
        {
        case ('w'):
        case ('a'):
        case ('s'):
        case ('d'):
            game.move(input);
            break;
        case ('1'):
        case ('2'):
        case ('3'):
            game.modifyLight(input - 49);
            break;
        case ('r'):
            game.rechargeLight();
            break;
        case ('t'):
            game = World(COLS / 2, LINES - 1); // THIS NEEDS a move constructor.
            break;
        case ('q'):
            return 0;
        }
    }
    endwin();
};