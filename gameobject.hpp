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
    unsigned lightRadius = 8; // 5 6 8 look good
    Player(char type, unsigned pos) : GameObject(type, pos) {}; //later move into cpp
};

#endif
