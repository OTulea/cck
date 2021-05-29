#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "world.h"

class Controller
{
    World model;

public:
    Controller(std::string base);
    void start(char race, const std::size_t seed, bool &restart);
    void interact(char type, int direc, std::size_t &seed);
};

#endif