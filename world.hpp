#ifndef WORLD
#define WORLD

#include <vector>
#include <iostream>
#include "cell.hpp"
#include "gameobject.hpp"

class World
{
    unsigned cols;
    std::vector<Cell> worldModel;
    Player player;

public:
    World(unsigned area, unsigned cols);
    void lightChange(char level);
    void updateVisibility(int radius);
    void interact(GameObject &object, char d);
    void interact(char d);
    void print();
};

#endif
