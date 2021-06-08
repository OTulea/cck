#ifndef WORLD
#define WORLD

#include <vector>
#include "gameobject.hpp"
struct Cell
{
    char type = '.';
    char contained = type;
    bool seen = false;
    bool visible = false;
    bool hasNo = true;
    bool hasEa = true;
    bool hasSo = true;
    bool hasWe = true;
};

class World
{
    int cols;
    std::vector<Cell> worldModel; //https://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster
    Player player;

public:
    World(int rows, int cols);
    void changeLightLevel(int level);
    void updateVisibility(int radius);
    void playerMove(char direc);
    int posUpdate(int pos, char direc);
    void print();
};

#endif
