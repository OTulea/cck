#include "curses.h"
#include "world.hpp"
#include "mytools.hpp"
#include "generation.hpp"

Cell::Cell(bool isFloor) : isFloor{isFloor}, contained{isFloor ? '.' : '#'} {};

void World::viewDims(int doubleXdim, int Ydim)
{
    oddColDim = doubleXdim % 2;
    viewportCols = doubleXdim / 2;
    viewportRows = Ydim;
}

bool World::posUpdate(int &pos, Direction direc) // this will eventually check if cell is empty
{
    if (direc == Direction::North && worldModel[pos - cols].isFloor)
        return pos -= cols;
    else if (direc == Direction::East && worldModel[pos + 1].isFloor)
        return ++pos;
    else if (direc == Direction::South && worldModel[pos + cols].isFloor)
        return pos += cols;
    else if (direc == Direction::West && worldModel[pos - 1].isFloor)
        return --pos;
    return false;
}

void World::print()
{
    clear();
    worldModel[playerPos].contained = '@';
    int xOff = (viewportCols - 1) / 2 - (playerPos % cols);
    int yOff = (viewportRows - 1) / 2 - (playerPos / cols);
    for (int i = 0; i < viewportRows; ++i)
    {
        for (int j = 0; j < viewportCols; ++j)
        {
            if (i - yOff >= 0 && i - yOff < worldModel.size() / cols && j - xOff >= 0 && j - xOff < cols)
            {
                addch(worldModel[(i - yOff) * cols + (j - xOff)].contained);
                worldModel[(i - yOff) * cols + (j - xOff)].contained = worldModel[(i - yOff) * cols + (j - xOff)].isFloor ? '.' : '#';
            }
            else
                addch(' ');
            addch(' ');
        }
        if (oddColDim)
            addch(' ');
    }
    refresh();
}

//Public
World::World(int cols, int rows) : cols{cols}
{
    Terrain terr = Terrain(cols, rows);
    for (const auto &elem : terr.wmap)
        worldModel.emplace_back(Cell(!elem.isWall));
    cave = terr.largestcave;
    playerPos = cave[randInt(0, cave.size() - 1)];
    viewDims(120, 30);
    print();
}

void World::attemptMove(Direction direc)
{
    if (posUpdate(playerPos, direc))
    {
        print();
    }
}
