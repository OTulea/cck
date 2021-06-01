#ifndef GAMEOBJECT
#define GAMEOBJECT

struct GameObject
{
    char type;
    unsigned pos;
    GameObject(char type, unsigned pos) : type{type}, pos{pos} {}; //later move into cpp
};

struct Player : public GameObject
{
    unsigned lightRadius = 2;
    unsigned fuel = 120;
    unsigned rate = 0;
    Player(char type, unsigned pos) : GameObject(type, pos){}; //later move into cpp
    void adjustLight(unsigned level)
    {
        if (fuel >= level * level)
        {
            rate = level * level;
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
