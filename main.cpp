#include <vector>
#include <iostream>
#include "world.hpp"
#include "curses.h"

#define COLOR_GREY COLOR_CYAN
//TODO//
// ignore resizing for now
// change light mechanic so shadows work
// add enemies
// add rngeneration
// add fuel pickups
// figure out why the first input gets eaten
// add fuel meter
// look into colours
// look into making diagonals and direction changing inputs possible

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

    // GAME SETUP
    World game(COLS / 2 * LINES, COLS / 2); //deal with resizing eventually
    char input;
    while (true)
    {
        input = getch();
        switch (input) // WHY DOES IT EAT THE FIRST INPUT;
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