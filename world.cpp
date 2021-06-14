#include "curses.h"
#include "world.hpp"
#include "mytools.hpp"
#include "generation.hpp"

bool World::posUpdate(int &pos, Direction direc)
{
    // this will eventually check if cell is empty
    if (direc == Direction::North && worldModel[pos - cols].isFloor)
    {
        pos -= cols;
        return true;
    }
    else if (direc == Direction::East && worldModel[pos + 1].isFloor)
    {
        ++pos;
        return true;
    }
    else if (direc == Direction::South && worldModel[pos + cols].isFloor)
    {
        pos += cols;
        return true;
    }
    else if (direc == Direction::West && worldModel[pos - 1].isFloor)
    {
        --pos;
        return true;
    }
    return false;
}

void World::createViewport()
{
    int xOff = min(max(0, playerPos % cols - (viewXdim - 1) / 2), cols - viewXdim);
    int yOff = min(max(0, playerPos / cols - (viewYdim - 1) / 2), worldModel.size() / cols - viewYdim);
    viewportAnchor = xOff + yOff * cols;
}

void World::print()
{
    clear();
    worldModel[playerPos].contained = '@';
    for (const auto &elem : enemies)
        worldModel[elem].contained = 'G';
    for (int i = 0; i < viewYdim; ++i)
        for (int j = 0; j < viewXdim; ++j)
        {
            int index = viewportAnchor + i * cols + j;
            addch(worldModel[index].contained);
            addch(' ');
            worldModel[index].contained = worldModel[index].isFloor ? '.' : '#';
        }
    refresh();
}

bool World::tooClose(int pos)
{
    int XDist = abso(pos % cols - playerPos % cols);
    int YDist = abso(pos / cols - playerPos / cols);
    if (XDist + YDist < 10)
        return true;
    for (const auto &elem : enemies)
    {
        XDist = abso(pos % cols - elem % cols);
        YDist = abso(pos / cols - elem / cols);
        if (XDist + YDist < 3)
            return true;
    }
    return false;
}

void World::spawnEnemies(int numEnemies)
{
    std::vector<int> possiblePostns = cave;
    for (int i = 0; i < numEnemies; ++i)
    {
        int index = randInt(0, possiblePostns.size() - 1);
        int tentativePos = possiblePostns[index];
        while (possiblePostns.size() && tooClose(tentativePos))
        {
            possiblePostns.erase(possiblePostns.begin() + index);
            index = randInt(0, possiblePostns.size() - 1);
            tentativePos = possiblePostns[index];
        }
        enemies.push_back(tentativePos);
    }
};

void World::iterate()
{
    for (auto &elem : enemies)
    {
        int direc = randInt(0, 3);
        while (!posUpdate(elem, Direction(direc)))
            direc = randInt(0, 3);
    }
    print();
}

//Public
World::World(int cols, int rows) : cols{cols}, viewXdim{60}, viewYdim{29}
{
    Terrain terr = Terrain(cols, rows);
    for (const auto &elem : terr.wmap)
        worldModel.emplace_back(Cell(!elem.isWall));
    cave = terr.largestcave;
    playerPos = cave[randInt(0, cave.size() - 1)];
    createViewport();
    spawnEnemies(15);
    print();
}

void World::move(Direction direc)
{
    if (posUpdate(playerPos, direc))
    {
        iterate();
        createViewport();
        print();
    }
}
