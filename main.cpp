#include "curses.h"
#include "world.hpp"

int main()
{
    // CURSES SETUP
    initscr(); // pdcurses starts in cbreak by default
    noecho();
    //halfdelay(1);
    curs_set(0);
    // GAME SETUP : WINDOWS TERMINAL is 120 by 30 base
    World game(500, 500);
    //GAME LOOP
    while (true)
    {
        if (is_termresized())
        {
            curs_set(0);
            resize_term(0, 0);
            resize_window(stdscr, LINES, COLS);
            game.viewDims(COLS, LINES);
            game.print();
        }
        char input = getch(); //this blocks until input received
        switch (input)
        {
        case ('w'):
            game.attemptMove(Direction::North);
            break;
        case ('d'):
            game.attemptMove(Direction::East);
            break;
        case ('s'):
            game.attemptMove(Direction::South);
            break;
        case ('a'):
            game.attemptMove(Direction::West);
            break;
        case ('r'):
            game = World(100, 100); // THIS NEEDS a move constructor. does it?
            break;
        case ('q'):
            return 0;
        }
    }
    endwin();
};