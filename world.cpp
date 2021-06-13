#include "curses.h"
#include "world.hpp"
#include "mytools.hpp"
#include "generation.hpp"

World::World(int cols, int rows) : cols{cols}
{
    Terrain terr = Terrain(cols, rows);
    for (const auto &elem : terr.wmap)
        worldModel.emplace_back(Cell(!elem.isWall));
    cave = terr.largestcave;
    playerPos = cave[randInt(0, cave.size() - 1)];
    updateVisibility();
    print();
}

bool World::ifisValid(int pos)
{
    if (pos >= 0 && pos < worldModel.size() && square(pos % cols - playerPos % cols) + square(pos / cols - playerPos / cols) < square(lightRadius))
    {
        worldModel[pos].visible = true;
        if (worldModel[pos].isFloor)
            return true;
    }
    return false;
}

void World::advance(int pos, int direc) // add checks for above or below centerpoint
{
    switch (direc)
    {
    case (0):
        if (ifisValid(pos - cols))
        {
            advance(pos - cols, direc);
            if (ifisValid(pos - cols - 1))
                advance(pos - cols - 1, direc);
            if (ifisValid(pos - cols + 1))
                advance(pos - cols + 1, direc);
        }
        break;
    case (1):
        if (ifisValid(pos + 1))
        {
            advance(pos + 1, direc);
            if (ifisValid(pos - cols + 1))
                advance(pos - cols + 1, direc);
            if (ifisValid(pos + cols + 1))
                advance(pos + cols + 1, direc);
        }
        break;
    case (2):
        if (ifisValid(pos + cols))
        {
            advance(pos + cols, direc);
            if (ifisValid(pos + cols - 1))
                advance(pos + cols - 1, direc);
            if (ifisValid(pos + cols + 1))
                advance(pos + cols + 1, direc);
        }
        break;
    case (3):
        if (ifisValid(pos - 1))
        {
            advance(pos - 1, direc);
            if (ifisValid(pos - cols - 1))
                advance(pos - cols - 1, direc);
            if (ifisValid(pos + cols - 1))
                advance(pos + cols - 1, direc);
        }
        break;
    }
}

bool World::posUpdate(int &pos, char d)
{
    // this will eventually check if cell is empty
    if (d == 'w' && worldModel[pos - cols].isFloor)
    {
        pos -= cols;
        return true;
    }
    else if (d == 'd' && worldModel[pos + 1].isFloor)
    {
        ++pos;
        return true;
    }
    else if (d == 's' && worldModel[pos + cols].isFloor)
    {
        pos += cols;
        return true;
    }
    else if (d == 'a' && worldModel[pos - 1].isFloor)
    {
        --pos;
        return true;
    }
    return false;
}

void World::updateVisibility() //add bounding box( max(0, pos -radius(cols+1)) to min(worldsize, pos + radius(col+1)) ))
{
    for (auto &elem : worldModel)
        if (elem.visible)
        {
            elem.seen = true;
            elem.visible = false;
        }
    worldModel[playerPos].visible = true;
    advance(playerPos, 0);
    advance(playerPos, 1);
    advance(playerPos, 2);
    advance(playerPos, 3);
}

void World::print()
{
    clear();
    worldModel[playerPos].contained = '@';
    for (auto &elem : worldModel)
    {
        if (elem.visible)
            attrset(COLOR_PAIR(3));
        else if (elem.seen)
            attrset(COLOR_PAIR(2));
        else
            attrset(COLOR_PAIR(1));
        // attrset(COLOR_PAIR(3));
        addch(elem.contained);
        addch(' ');
        elem.contained = elem.visible ? elem.isFloor ? '.' : '#' : elem.contained;
    }
    auto index = 3;
    for (auto i = 0; i < fuel; ++i)
    {
        if (rate && (!((fuel - i) % 4) || rate == 1))
            index = i % (2 * rate) >= rate ? 4 : 3;

        attrset(COLOR_PAIR(index));
        addch(177);
    }
    refresh();
}

void World::move(char direction)
{
    if (posUpdate(playerPos, direction))
    {
        fuel = fuel <= rate ? 0 : fuel - rate;
        if (!fuel)
        {
            rate = 0;
            lightRadius = 2;
        }
        updateVisibility();
        print();
    }
}

void World::modifyLight(int level)
{
    if (rate != square(level) && fuel >= square(level))
    {
        rate = square(level);
        lightRadius = level ? level * 4 : 2;
        updateVisibility();
        print();
    }
}

void World::rechargeLight()
{
    if (fuel != 120)
    {
        fuel = 120;
        print();
    }
}
