#ifndef WORLD
#define WORLD

#include <vector>

struct Cell
{
    bool isFloor;
    char contained;
    bool seen = false;
    bool visible = false;
    Cell(bool isFloor) : isFloor{isFloor}, contained{isFloor ? '.' : '#'} {};
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
    int viewXdim;
    int viewYdim;
    int viewportAnchor;
    std::vector<Cell> worldModel; //https://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster
    std::vector<int> cave;
    std::vector<int> enemies;
    bool posUpdate(int &pos, Direction direc);
    void print();
    bool tooClose(int pos);
    void spawnEnemies(int numEnemies);
    void iterate();
    void createViewport();

public:
    World(int cols, int rows);
    void move(Direction direc);
};

#endif
