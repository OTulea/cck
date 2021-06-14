#include "curses.h"
#include "world.hpp"

#define COLOR_GREY 65

int main()
{
    // CURSES SETUP
    initscr(); // pdcurses starts in cbreak by default
    noecho();
    curs_set(0);
    // GAME SETUP : WINDOWS TERMINAL is 120 by 30 base
    World game(100, 100);
    //GAME LOOP
    while (true)
    {
        char input = getch(); //this blocks until input received
        switch (input)
        {
        case ('w'):
            game.move(Direction::North);
            break;
        case ('a'):
            game.move(Direction::West);
            break;
        case ('s'):
            game.move(Direction::South);
            break;
        case ('d'):
            game.move(Direction::East);
            break;
        case ('r'):
            game = World(100, 100); // THIS NEEDS a move constructor.
            break;
        case ('q'):
            return 0;
        }
    }
    endwin();
};