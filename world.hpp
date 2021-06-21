#ifndef WORLD
#define WORLD

#include <vector>

struct Cell
{
    bool isFloor;
    char contained;
    bool seen = false;
    bool visible = false;
    Cell(bool isFloor);
};

enum Direction
{
    North,
    South,
    East,
    West
};

class World
{
    int cols;
    int playerPos;
    bool oddColDim;
    int viewportCols;
    int viewportRows;
    int viewportAnchor;
    std::vector<Cell> worldModel; //https://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster
    std::vector<int> cave;
    bool posUpdate(int &pos, Direction direc);

public:
    void print();
    void viewDims(int doubleXdim, int Ydim);
    World(int cols, int rows);
    void attemptMove(Direction direc);
};

#endif
