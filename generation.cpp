#include "mytools.hpp"
#include "generation.hpp"

Terrain::Terrain(int cols, int rows) : cols{cols}, area{rows * cols}, lowerMutableBound{cols + 1}, upperMutableBound{area - cols - 1}, wmap(area, basicCell())
{
    for (int i = 0; i < cols; ++i)
        wmap[i].isMutable = false;
    for (int i = cols - 1; i < area; i += cols)
        wmap[i].isMutable = false;
    for (int i = area - cols; i < area; ++i)
        wmap[i].isMutable = false;
    for (int i = 0; i <= area - cols; i += cols)
        wmap[i].isMutable = false;
    createCaves(42, 5, 4, 45, 55);
}

void Terrain::addNoise(int noiseRatio)
{
    for (int i = lowerMutableBound; i < upperMutableBound; ++i)
    {
        int Xdist = abso(i % cols - cols / 2);
        int Ydist = 2 * abso(i / cols - area / cols / 2);
        if (wmap[i].isMutable && (randInt((Xdist + Ydist), 100) < noiseRatio - 20 || randInt(0, 100) < noiseRatio))
            wmap[i].isWall = false;
    }
}

void Terrain::iterateAutomata(int wallBreakpoint)
{
    for (int i = lowerMutableBound; i < upperMutableBound; ++i)
        if (wmap[i].isMutable)
        {
            int numWallnghbrs = 0;
            if (wmap[i - cols - 1].isWall)
                ++numWallnghbrs;
            if (wmap[i - cols].isWall)
                ++numWallnghbrs;
            if (wmap[i - cols + 1].isWall)
                ++numWallnghbrs;
            if (wmap[i - 1].isWall)
                ++numWallnghbrs;
            if (wmap[i + 1].isWall)
                ++numWallnghbrs;
            if (wmap[i + cols - 1].isWall)
                ++numWallnghbrs;
            if (wmap[i + cols].isWall)
                ++numWallnghbrs;
            if (wmap[i + cols + 1].isWall)
                ++numWallnghbrs;
            wmap[i].newVal = numWallnghbrs > wallBreakpoint ? true : false;
        }
    for (auto &elem : wmap)
        elem.isWall = elem.isMutable ? elem.newVal : elem.isWall;
}

void Terrain::fillDeadEnds(int smoothingBreakpoint)
{
    for (int i = lowerMutableBound; i < upperMutableBound; ++i)
        if (!wmap[i].isWall)
        {
            int numWallnghbrs = 0;
            if (wmap[i - cols - 1].isWall)
                ++numWallnghbrs;
            if (wmap[i - cols].isWall)
                ++numWallnghbrs;
            if (wmap[i - cols + 1].isWall)
                ++numWallnghbrs;
            if (wmap[i - 1].isWall)
                ++numWallnghbrs;
            if (wmap[i + 1].isWall)
                ++numWallnghbrs;
            if (wmap[i + cols - 1].isWall)
                ++numWallnghbrs;
            if (wmap[i + cols].isWall)
                ++numWallnghbrs;
            if (wmap[i + cols + 1].isWall)
                ++numWallnghbrs;
            if (numWallnghbrs > smoothingBreakpoint)
            {
                wmap[i].isWall = true;
                i = i - cols - 2; // -2 because for loop will auto increment after this line, this sets to the earliest neighbour.
            }
        }
}

void Terrain::visit(const int pos, std::vector<int> &store)
{
    wmap[pos].wasVisited = true;
    const int cellNghbrs[]{pos - cols, pos + 1, pos + cols, pos - 1};
    for (const int &elem : cellNghbrs)
        if (!wmap[elem].isWall && !wmap[elem].wasVisited && !wmap[elem].taken)
        {
            wmap[elem].taken = true;
            store.push_back(elem);
        }
}

void Terrain::flood(const int pos, std::vector<int> &store)
{
    store.push_back(pos);
    int numVisited = 0;
    while (numVisited < store.size())
    {
        visit(store[numVisited], store);
        ++numVisited;
    }
}

bool Terrain::areUnvisited(int &curPos)
{
    for (; curPos < upperMutableBound; ++curPos)
        if (!wmap[curPos].isWall && !wmap[curPos].wasVisited)
            return true;
    return false;
}

int Terrain::floodAllSmallerCaves()
{
    std::vector<int> max;
    std::vector<int> newvec;
    int curPos = lowerMutableBound;
    while (wmap[curPos].isWall)
        ++curPos;
    flood(curPos, max);
    while (areUnvisited(curPos))
    {
        flood(curPos, newvec);
        if (max.size() > newvec.size())
            for (const int &elem : newvec)
                wmap[elem].isWall = true;
        else
        {
            for (const int &elem : max)
                wmap[elem].isWall = true;
            max = newvec;
        }
        newvec.clear();
    }
    largestcave = max;
    return (max.size() * 100) / area;
}

void Terrain::createCaves(int noiseRatio, int wallBreakpoint, int smoothingBreakpoint, int lowerFloorRatio, int upperFloorRatio)
{
    addNoise(noiseRatio);
    iterateAutomata(wallBreakpoint);
    fillDeadEnds(smoothingBreakpoint);
    int floorRatio = floodAllSmallerCaves();
    if (floorRatio < lowerFloorRatio || floorRatio > upperFloorRatio)
        *this = Terrain(cols, area / cols);
}