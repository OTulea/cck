#include "objecTree.h"

GameObject::GameObject(char type) : type{type} {};

char GameObject::getType()
{
    return type;
}

Gold::Gold(std::size_t inVal) : GameObject('G'), value{inVal}, isTakeable{value == 6 ? false : true} {}

int Gold::getValue()
{
    return value;
}

void Gold::liberate()
{
    isTakeable = true;
}
bool Gold::canTake()
{
    return isTakeable;
}

Character::Character(char race) : GameObject(race), gold{1}, hp{0}, atk{0}, def{0}
{
    switch (race)
    {
    case 'V':
        hp = 50;
        atk = 25;
        def = 25;
        break;
    case 'W':
        hp = 120;
        atk = 30;
        def = 5;
        break;
    case 'T':
        hp = 120;
        atk = 25;
        def = 15;
        break;
    case 'N':
        hp = 70;
        atk = 5;
        def = 10;
        break;
    case 'M':
        gold = 4;
        hp = 30;
        atk = 70;
        def = 5;
        break;
    case 'D':
        hp = 150;
        atk = 20;
        def = 20;
        break;
    case 'X':
        hp = 50;
        atk = 35;
        def = 25;
        break;
    case 'h':
        gold = 0;
        hp = 140;
        atk = 20;
        def = 20;
        break;
    case 'd':
        gold = 0;
        hp = 100;
        atk = 20;
        def = 30;
        break;
    case 'e':
        gold = 0;
        hp = 140;
        atk = 30;
        def = 10;
        break;
    case 'o':
        gold = 0;
        hp = 180;
        atk = 30;
        def = 25;
        break;
    }
}

Dragon::Dragon(std::size_t tdirec, char race) : Character(race), tDirec{tdirec} {}

Player::Player(char race) : Character(race), maxHP{hp}, baseAtk{atk}, baseDef{def} {};
