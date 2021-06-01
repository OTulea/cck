#include <iostream>
#include "world.hpp"
#include "curses.h"

World::World(unsigned area, unsigned cols) : cols{cols}, worldModel(area, Cell()), player('@', 0)
{
    for (unsigned i = 0; i < area; ++i)
    {
        worldModel[i].hasNo = (i >= cols) ? true : false;
        worldModel[i].hasEa = (i % cols < cols - 1) ? true : false;
        worldModel[i].hasSo = (i < area - cols) ? true : false;
        worldModel[i].hasWe = (i % cols) ? true : false;
    }
    updateVisibility(player.lightRadius);
    print();
}

auto abso = [](int a)
{
    return a < 0 ? -a : a;
};

auto square = [](int a)
{
    return a * a;
};

void World::updateVisibility(int radius)
{
    int XDist;
    int YDist;
    for (auto i = 0; i < worldModel.size(); ++i)
    {
        XDist = player.pos % cols - i % cols;
        YDist = player.pos / cols - i / cols;
        worldModel[i].visible = square(abso(XDist)) + square(abso(YDist)) < square(radius) ? true : false;
        worldModel[i].seen = worldModel[i].visible ? true : worldModel[i].seen;
    }
}

void World::interact(GameObject &object, char d)
{
    // this will eventually check if cell is empty
    if (d == 'w' && worldModel[object.pos].hasNo)
        object.pos -= cols;
    if (d == 'd' && worldModel[object.pos].hasEa)
        ++object.pos;
    if (d == 's' && worldModel[object.pos].hasSo)
        object.pos += cols;
    if (d == 'a' && worldModel[object.pos].hasWe)
        --object.pos;
}

void World::interact(char d)
{
    interact(player, d);
    updateVisibility(player.lightRadius);
    print();
}

void World::print() //convert to just outputting a string so i can manage curses in main?
{
    worldModel[player.pos].contained = player.type;
    for (auto &elem : worldModel)
    {
        if (elem.visible)
            attrset(COLOR_PAIR(3));
        else if (elem.seen)
            attrset(COLOR_PAIR(2));
        else
            attrset(COLOR_PAIR(1));
        addch(elem.contained);
        addch(' ');

        elem.contained = elem.visible ? elem.type: elem.contained;
    }
}