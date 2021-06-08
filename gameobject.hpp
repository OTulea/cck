#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "mytools.hpp"

struct GameObject
{
    char type;
    int pos;
    GameObject(char type, int pos) : type{type}, pos{pos} {}; //later move into cpp
};

struct Player : public GameObject
{
    int lightRadius = 2;
    int fuel = 120;
    int rate = 0;
    Player(char type, int pos) : GameObject(type, pos){}; //later move into cpp
    void adjustLight(int level)
    {
        if (fuel >= tool::square(level))
        {
            rate = tool::square(level);
            lightRadius = level ? level * 4 : 2;
        }
    }
    void burn()
    {
        fuel = fuel <= rate ? 0 : fuel - rate;
        if (!fuel)
            adjustLight(0);
    }
};

#endif
