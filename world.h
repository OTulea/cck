#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "display.h"
#include "cell.h"

class World
{
    Display view;
    std::vector<Cell> curFloor;
    std::vector<std::vector<std::size_t>> chambers;
    std::size_t floorNum;
    Cell *pc;
    Cell *stairs;
    Cell *compass; // NOT CODED YET
    Cell *suit;    // NOT CODED YET
    bool merchantsHostile;
    std::vector<Cell *> enemies; // keep in mind merchant special case

public:
    World(std::string base);
    void generateFloor(std::size_t seed);
    void setRace(char race);
    void clearFloor();
    bool attack(int direc, std::size_t &seed);
    bool use(int direc);
    bool move(int direc);
    void resolve(std::size_t &seed);
    void print();
};

#endif