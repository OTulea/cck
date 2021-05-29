#ifndef WORLD
#define WORLD

#include <vector>
#include <iostream>
#include "cell.hpp"
#include "gameobject.hpp"

class World
{
    unsigned cols;
    unsigned rows;
    std::vector<Cell> world;
    std::vector<DisplayCell> map;
    // std::vector<GameObject> objects;
    // std::vector<GameObject> enemies;
    GameObject player;

public:
    World(unsigned area, unsigned cols, unsigned rows);
    void interact(GameObject &object, char d);
    void interact(char d);
    void print();
};

#endif
