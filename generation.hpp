#ifndef GENERATION
#define GENERATION

#include <vector>

struct cell
{
    bool isWall = true;
    bool isMutable = true;
    bool newVal = true;
    bool wasVisited = false;
    bool taken = false;
};

struct terrain
{
    unsigned cols;
    std::vector<cell> wmap;
    terrain(unsigned area, unsigned cols, unsigned ratio);
    void addNoise(unsigned ratio);
    void iterateAutomata();
    void fillDeadEnds();
    void visit(unsigned pos, std::vector<unsigned> &store);
    void flood(unsigned pos, std::vector<unsigned> &store);
    bool areUnvisited(unsigned &index);
    void floodMiniCaves();
};

#endif
