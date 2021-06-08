#include "world.hpp"
#include "curses.h"

#define COLOR_GREY 65
//TODO//
// change light mechanic so shadows work
// add enemies
// add rngeneration
// add fuel pickups
// look into colours
// add feedback for everything
// gameplan is to escape 
// whats my core gameplay?
// look into style guides.

int main()
{
    // CURSES SETUP
    initscr();     // pdcurses starts in cbreak by default
    start_color(); // recompile curses with wide chars for more colours
    noecho();
    curs_set(0);
    init_color(COLOR_GREY, 400, 400, 400);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_GREY, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    // GAME SETUP
    // WINDOWS TERMINAL is 120 by 30;
    World game(COLS / 2 * (LINES - 1), COLS / 2); //deal with resizing eventually
        while (true)
    {
        char input = getch(); //this blocks until input received
        switch (input)
        {
        case ('w'):
        case ('a'):
        case ('s'):
        case ('d'):
            game.playerMove(input);
            break;
        case ('1'):
        case ('2'):
        case ('3'):
            game.changeLightLevel('1' - 49);
            break;
        }
    }
    endwin();
};