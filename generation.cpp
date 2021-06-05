#include "randint.hpp"
#include "generation.hpp"

terrain::terrain(unsigned area, unsigned cols, unsigned ratio) : cols{cols}, wmap(area, cell())
{
    for (unsigned i = 0; i < wmap.size(); ++i)
        if (i / cols < 1 || i % cols < 1 || i % cols == cols - 1 || i > area - cols)
            wmap[i].isMutable = false;
    addNoise(42);
    iterateAutomata();
    fillDeadEnds();
    floodMiniCaves();
}

int abso(int a)
{
    return a < 0 ? -a : a;
}

void terrain::addNoise(unsigned ratio)
{
    for (unsigned i = 0; i < wmap.size(); ++i)
    {
        int Xdist = abso(i % cols - cols / 2);
        int Ydist = 2 * abso(i / cols - (wmap.size() / cols) / 2);
        if (wmap[i].isMutable && (randInt((Xdist + Ydist), 100) < ratio - 20 && randInt(0, 100) < ratio))
            wmap[i].isWall = false;
    }
}

void terrain::iterateAutomata()
{
    for (auto i = cols + 1; i < wmap.size() - cols - 1; ++i)
        if (wmap[i].isMutable)
        {
            unsigned numWallnghbrs = 0;
            for (int k = -1; k <= 1; ++k)
                for (int j = -1; j <= 1; ++j)
                    if (wmap[i + k * cols + j].isWall && !(j == 0 && k == 0))
                        ++numWallnghbrs;
            wmap[i].newVal = numWallnghbrs >= 6 ? true : false;
        }
    for (auto &elem : wmap)
        if (elem.isMutable)
            elem.isWall = elem.newVal;
}

void terrain::fillDeadEnds()
{
    for (auto i = cols + 1; i < wmap.size() - cols - 1; ++i)
        if (!wmap[i].isWall)
        {
            unsigned numWallnghbrs = 0;
            for (int k = -1; k <= 1; ++k)
                for (int j = -1; j <= 1; ++j)
                    if (wmap[i + k * cols + j].isWall && !(j == 0 && k == 0))
                        ++numWallnghbrs;
            if (numWallnghbrs >= 5)
            {
                wmap[i].isWall = true;
                i = i - cols - 2;
            }
        }
}

bool terrain::areUnvisited(unsigned &index)
{
    for (; index < wmap.size() - cols; ++index)
        if (!wmap[index].isWall && !wmap[index].wasVisited)
            return true;
    return false;
}

void terrain::visit(unsigned pos, std::vector<unsigned> &store)
{
    wmap[pos].wasVisited = true;
    const unsigned cellNghbrs[]{pos - cols, pos + 1, pos + cols, pos - 1};
    for (auto elem : cellNghbrs)
        if (!wmap[elem].isWall && !wmap[elem].wasVisited && !wmap[elem].taken)
        {
            wmap[elem].taken = true;
            store.push_back(elem);
        }
}

void terrain::flood(unsigned pos, std::vector<unsigned> &store)
{
    store.push_back(pos);
    unsigned numVisited = 0;
    while (numVisited < store.size())
    {
        visit(store[numVisited], store);
        ++numVisited;
    }
}

void terrain::floodMiniCaves()
{
    std::vector<unsigned> max;
    std::vector<unsigned> newvec;
    unsigned cur = cols;
    while (wmap[cur].isWall)
        ++cur;
    flood(cur, max);
    while (areUnvisited(cur))
    {
        flood(cur, newvec);
        if (max.size() > newvec.size())
            for (const auto &elem : newvec)
                wmap[elem].isWall = true;
        else
        {
            for (const auto &elem : max)
                wmap[elem].isWall = true;
            max = newvec;
        }
        newvec.clear();
    }
}