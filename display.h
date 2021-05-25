#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <ostream>

class Display
{
    std::vector<char> dp;
    char race;
    std::size_t floor;
    std::size_t gold;
    std::size_t hp;
    std::size_t atk;
    std::size_t def;
    std::string currentAction;

public:
    Display(const std::string base);
    void notify(std::size_t pos, char type);                                   // updates a single cell
    void setRace(char race, std::size_t hp, std::size_t atk, std::size_t def); //set the interal storage of player stats
    void notify(std::size_t hp, std::size_t atk, std::size_t def);             //updates internal stat storage for potions or combat results
    void notify(std::size_t gold);                                             //updates if gold is accquired
    void notify(std::string newAction);                                        //updates currentAction
    void notifyAppend(std::string newAction);               //updates currentAction (for enemies)
    void advanceFloor(std::size_t batk, std::size_t bdef);                     //sets up display for the next floor
    void reset();                                                              //called when the player resets the game
    friend std::ostream &operator<<(std::ostream &os, const Display &display);
};

#endif