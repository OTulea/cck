#include <cstddef>

class GameObject
{
    char type;
public:
    char getType();
    GameObject(char type);
};

class Gold : public GameObject
{
    std::size_t value;
    bool isTakeable;
public:
    int getValue();
    void liberate();
    bool canTake();
    Gold(std::size_t value);
};

class Character : public GameObject
{

public: // eventually add accessors and set to private
    bool canAct; // only useful for enemies, might want to move?
    std::size_t gold;
    std::size_t hp;
    std::size_t atk;
    std::size_t def;
    Character(char race);
};

class Dragon : public Character {
    public:
    std::size_t tDirec;
    Dragon(std::size_t tdirec, char race);
};

class Player : public Character
{
public:
    std::size_t maxHP;
    std::size_t baseAtk;
    std::size_t baseDef;
    Player(char race);
};