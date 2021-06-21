#include "mytools.hpp"
#include "generation.hpp"

Terrain::Terrain(int cols, int rows) : cols{cols}, area{rows * cols}, wmap(area, basicCell())
{
    for (int i = 0; i < cols; ++i)
        wmap[i].isMutable = false;
    for (int i = cols - 1; i < area; i += cols)
        wmap[i].isMutable = false;
    for (int i = area - cols; i < area; ++i)
        wmap[i].isMutable = false;
    for (int i = 0; i <= area - cols; i += cols)
        wmap[i].isMutable = false;
    createCaves(43, 1, 5, 4, 35, 50);
}

void Terrain::addNoise(int noiseRatio)
{
    for (auto &elem : wmap)
        elem.isWall = !elem.isMutable || randInt(1, 100) >= noiseRatio;
}

int Terrain::countNeighbours(int pos)
{
    return wmap[pos - cols - 1].isWall +
           wmap[pos - cols].isWall +
           wmap[pos - cols + 1].isWall +
           wmap[pos - 1].isWall +
           wmap[pos + 1].isWall +
           wmap[pos + cols - 1].isWall +
           wmap[pos + cols].isWall +
           wmap[pos + cols + 1].isWall;
}

void Terrain::iterateAutomata(int wallBreakpoint)
{
    for (int i = 0; i < area; ++i)
        if (wmap[i].isMutable)
            wmap[i].newVal = countNeighbours(i) > wallBreakpoint;
    for (auto &elem : wmap)
        elem.isWall = elem.newVal;
}

void Terrain::fillDeadEnds(int smoothingBreakpoint)
{
    for (int i = 0; i < area; ++i)
        if (!wmap[i].isWall && countNeighbours(i) > smoothingBreakpoint)
        {
            wmap[i].isWall = true;
            i = i - cols - 2; // -2 because for loop will auto increment after this line, go to earliest neighbour that could be affected by above change
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
    for (int numVisited = 0; numVisited < store.size(); ++numVisited)
        visit(store[numVisited], store);
}

bool Terrain::areUnvisited(int &curPos)
{
    for (; curPos < area; ++curPos)
        if (!wmap[curPos].isWall && !wmap[curPos].wasVisited)
            return true;
    return false;
}

int Terrain::floodAllSmallerCaves()
{
    std::vector<int> max;
    std::vector<int> newvec;
    int curPos = 0;
    if (areUnvisited(curPos))
        flood(curPos, max);
    while (areUnvisited(curPos))
    {
        flood(curPos, newvec);
        for (const int &elem : max.size() > newvec.size() ? newvec : max)
            wmap[elem].isWall = true;
        max = max.size() > newvec.size() ? max : newvec;
        newvec.clear();
    }
    largestcave = max;
    return (max.size() * 100) / area;
}

void Terrain::createCaves(int noiseRatio, int iterations, int wallBreakpoint, int smoothingBreakpoint, int lowerFloorRatio, int upperFloorRatio)
{
    addNoise(noiseRatio);
    for (int i = 0; i < iterations; ++i)
        iterateAutomata(wallBreakpoint);
    fillDeadEnds(smoothingBreakpoint);
    int floorRatio = floodAllSmallerCaves();
    if (floorRatio < lowerFloorRatio || floorRatio > upperFloorRatio) //switch random to combination to avoid bounds
        *this = Terrain(cols, area / cols);
}