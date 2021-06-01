#include <iostream>
#include "world.hpp"
#include "curses.h"
#include "randInt.hpp"

World::World(unsigned area, unsigned cols) : cols{cols}, worldModel(area, Cell()), player('@', randInt(0, worldModel.size()))
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

void World::lightChange(char level)
{
    if (player.rate != level - 49 * level - 49)
    {
        player.adjustLight(level - 49);
        updateVisibility(player.lightRadius);
        print();
    }
}

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
        worldModel[i].visible = square(XDist) + square(YDist) < square(radius) ? true : false;
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
    player.burn();
    updateVisibility(player.lightRadius);
    print();
}

void World::print() //convert to just outputting a string so i can manage curses in main?
{
    clear();
    //move(0, 0);
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
        elem.contained = elem.visible ? elem.type : elem.contained;
    }
    unsigned index = 2;
    for (unsigned i = 0; i < player.fuel; ++i)
    {
        if (player.rate && (!((player.fuel - i) % 4) || player.rate == 1))
            index = i % (2 * player.rate) >= player.rate ? 4 : 3;

        attrset(COLOR_PAIR(index));
        addch(177);
    }
    refresh();
}
