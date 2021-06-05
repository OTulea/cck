#ifndef WORLD
#define WORLD

#include <vector>
#include <iostream>
#include "cell.hpp"
#include "gameobject.hpp"

// World Ideas: 
// large map, distinct zones requiring different generators (caves & tunnels, ruins, forests)
// have predefined features placed randomly, connected by generation
// have enterable buildings with their own distinct maps
// 
//
//

class World
{
    unsigned cols;
    std::vector<Cell> worldModel; //https://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster
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
