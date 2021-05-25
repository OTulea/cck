#ifndef CELL_H
#define CELL_H

#include <vector>
#include <memory>
#include "objecTree.h"

class Cell
{
public:
    char type;
    std::size_t pos;
    std::unique_ptr<GameObject> ptr;
    std::vector<Cell*> neighbours;
    Cell(char type, std::size_t pos);
    void setNeighbour(Cell *cell, std::size_t direc);
    std::size_t enemyMove(int direc, std::size_t &seed);
    bool enemyAttack(int direc, std::size_t &seed, std::string &action);
    bool playerAttack(int direc, std::string &action);
    bool playerMove(int direc, std::string &action);
    bool use(int direc, std::string &action);
};

#endif