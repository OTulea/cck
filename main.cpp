#include <vector>
#include <iostream>
#include "world.hpp"
#include "curses.h"

int main(int argc, char *argv[])
{
    initscr();            // pdcurses starts in cbreak by default
    keypad(stdscr, TRUE); // this lets special characters like F2 be interpreted
    noecho();
    curs_set(0); // hides cursor
    World game(COLS * LINES, COLS, LINES);
    game.print();
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
            game.print();
            refresh();
            napms(16);
            flushinp();
            break;
        }
    }
    endwin();
};