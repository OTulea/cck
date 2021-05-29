#include <iostream>
#include "world.hpp"
#include "curses.h"

World::World(unsigned area, unsigned cols, unsigned rows) : cols{cols}, rows{rows}, world(area, Cell()), map(area, DisplayCell()), player{GameObject('@', 0)}
{
    for (unsigned i = 0; i < area; ++i)
    {
        world[i].hasNo = (i >= cols) ? true : false;
        world[i].hasEa = (i % cols < cols - 1) ? true : false;
        world[i].hasSo = (i < area - cols) ? true : false;
        world[i].hasWe = (i % cols) ? true : false;
    }
}

void World::interact(GameObject &object, char d)
{
    // this will eventually check if cell is empty
    if (d == 'w' && world[object.pos].hasNo)
        object.pos -= cols;
    if (d == 'd' && world[object.pos].hasEa)
        ++object.pos;
    if (d == 's' && world[object.pos].hasSo)
        object.pos += cols;
    if (d == 'a' && world[object.pos].hasWe)
        --object.pos;
}

void World::interact(char d)
{
    interact(this->player, d);
}

void World::print()
{
    this->map[this->player.pos].overwrite = this->player.type;
    for (unsigned i = 0; i < this->map.size(); ++i)
    {
        addch(this->map[i].overwrite);
        this->map[i].overwrite = this->map[i].type;
    }
}