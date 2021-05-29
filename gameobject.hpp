#ifndef GAMEOBJECT
#define GAMEOBJECT

struct GameObject
{
    char type;
    unsigned pos;
    GameObject(char type, unsigned pos) : type{type}, pos{pos} {}; //later move into cpp
};

#endif
