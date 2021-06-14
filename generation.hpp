#ifndef GENERATION
#define GENERATION

#include <vector>

struct basicCell
{
    bool isWall = true;
    bool isMutable = true;
    bool newVal = true;
    bool wasVisited = false;
    bool taken = false;
};

struct Terrain
{
    int cols;
    int area;
    int lowerMutableBound;
    int upperMutableBound;
    std::vector<basicCell> wmap;
    std::vector<int> largestcave;
    Terrain(int cols, int rows);
    void addNoise(int ratio);
    void iterateAutomata(int wallBreakpoint);
    void fillDeadEnds(int smoothingBreakpoint);
    void visit(const int pos, std::vector<int> &store);
    void flood(const int pos, std::vector<int> &store);
    bool areUnvisited(int &curPos);
    int floodAllSmallerCaves();
    void createCaves(int noiseRatio, int iterations, int wallBreakpoint, int smoothingBreakpoint, int lowerFloorRatio, int upperFloorRatio);
};

#endif
