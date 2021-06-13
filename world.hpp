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

class World
{
    int cols;
    int lightRadius = 2;
    int fuel = 120;
    int rate = 0;
    int playerPos;
    std::vector<Cell> worldModel; //https://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster
    std::vector<int> cave;
    std::vector<int> enemies;
    bool ifisValid(int pos);
    void advance(int pos, int direc);
    bool posUpdate(int &pos, char direc);
    void updateVisibility();
    void print();
    bool tooClose(int pos);
    void spawnEnemies(int numEnemies);
    void iterate();

public:
    World(int cols, int rows);
    void move(char direction);
    void modifyLight(int level);
    void rechargeLight();
};

#endif
