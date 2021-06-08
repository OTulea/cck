#include "world.hpp"
#include "curses.h"
#include "mytools.hpp"

World::World(int rows, int cols) : cols{cols}, worldModel(rows * cols, Cell()), player('@', 0)
{
    for (auto i = 0; i < cols; ++i)
        worldModel[i].hasNo = false;
    for (auto i = cols - 1; i < worldModel.size(); i += cols)
        worldModel[i].hasEa = false;
    for (auto i = worldModel.size() - cols; i < worldModel.size(); ++i)
        worldModel[i].hasSo = false;
    for (auto i = 0; i <= worldModel.size() - cols; i += cols)
        worldModel[i].hasWe = false;
    updateVisibility(player.lightRadius);
    print();
}

void World::changeLightLevel(int level)
{
    if (player.rate != level * level)
    {
        player.adjustLight(level);
        updateVisibility(player.lightRadius);
        print();
    }
}

void World::updateVisibility(int radius) //add bounding box( max(0, pos -radius(cols+1)) to min(worldsize, pos + radius(col+1)) ))
{
    for (auto i = 0; i < worldModel.size(); ++i)
    {
        int XDist = player.pos % cols - i % cols;
        int YDist = player.pos / cols - i / cols;
        worldModel[i].visible = tool::square(XDist) + tool::square(YDist) < tool::square(radius) ? true : false;
        worldModel[i].seen = worldModel[i].visible ? true : worldModel[i].seen;
    }
}

int World::posUpdate(int pos, char d)
{
    // this will eventually check if cell is empty
    if (d == 'w' && worldModel[pos].hasNo)
        return pos -= cols;
    else if (d == 'd' && worldModel[pos].hasEa)
        return ++pos;
    else if (d == 's' && worldModel[pos].hasSo)
        return pos += cols;
    else if (worldModel[pos].hasWe)
        return --pos;
    else
        return pos;
}

void World::playerMove(char direc)
{
    posUpdate(player.pos, direc);
    player.burn();
    updateVisibility(player.lightRadius);
    print();
}

void World::print()
{
    clear();
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
    for (auto i = 0; i < player.fuel; ++i)
    {
        if (player.rate && (!((player.fuel - i) % 4) || player.rate == 1))
            attrset(COLOR_PAIR(i % (2 * player.rate) >= player.rate ? 4 : 3));
        addch(177);
    }
    refresh();
}
