#include "controller.h"

Controller::Controller(std::string base) : model{World(base)} {}

void Controller::start(const char race, const std::size_t seed, bool &restart)
{
    if (restart)
    {
        model.clearFloor();
        restart = false;
    }

    model.generateFloor(seed);
    model.setRace(race);
    model.print();
}

void Controller::interact(const char type, const int direc, std::size_t &seed)
{
    bool success = false;

    if (type == 'a')
    {
        if (model.attack(direc, seed))
            success = true;
    }
    else if (type == 'u') // Add check if dead
    {
        if (model.use(direc))
            success = true;
    }
    else if (model.move(direc))
        success = true;

    if (success)
    {
        model.resolve(seed);
    }
    model.print();
}