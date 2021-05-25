#include "display.h"

Display::Display(const std::string base) : race{'?'}, floor{1}, gold{0}, hp{0}, atk{0}, def{0}
{
    for (const char &c : base)
    {
        dp.push_back(c);
    }
}

void Display::notify(const std::size_t pos, char type)
{
    if (type == 'h' || type == 'd' || type == 'e' || type == 'o')
        type = '@';
    dp.at(pos) = type;
}

void Display::setRace(const char nrace, const std::size_t nhp, const std::size_t natk, const std::size_t ndef) //set the interal storage of player stats
{
    race = nrace;
    hp = nhp;
    atk = natk;
    def = ndef;
}

void Display::notify(const std::size_t nhp, const std::size_t natk, const std::size_t ndef) //updates internal stat storage for potions or combat results
{
    hp = nhp;
    atk = natk;
    def = ndef;
}

void Display::notify(std::size_t ngold) {
    gold = ngold;
}

void Display::notify(std::string newAction) {
    currentAction = newAction;
}

void Display::notifyAppend(std::string newAction){
    currentAction += newAction;
}

void Display::advanceFloor(const std::size_t batk, const std::size_t bdef) //sets up display for the next floor
{
    for (auto &elem : dp)
    {
        if (elem != '|' || elem != '-' || elem != '+' || elem != '#' || elem != ' ')
            elem = '.';
    }
    ++floor;
    atk = batk;
    def = bdef;
}

void Display::reset() //called when the player resets the game
{
    for (auto &elem : dp)
    {
        if (elem != '|' && elem != '-' && elem != '+' && elem != '#' && elem != ' ')
            elem = '.';
    }
    floor = 1;
    gold = 0;
    currentAction = "";
}

std::ostream &operator<<(std::ostream &os, const Display &display)
{
    auto counter = 1;
    for (const auto &elem : display.dp)
    {
        os << elem;
        if (counter % 79 == 0)
            os << '\n';
        ++counter;
    }
    std::string fullRace;
    if (display.race == 'h')
        fullRace = "Human";
    else if (display.race == 'd')
        fullRace = "Dwarf";
    else if (display.race == 'e')
        fullRace = "Elf";
    else
        fullRace = "Orc";
    std::string justified = "Race: " + fullRace + " Gold: " + std::to_string(display.gold);
    justified.append((69 - justified.length()), ' ');
    os << justified << "Floor " << display.floor << "   \n";
    os << "HP: " << display.hp << "\n";
    os << "Atk: " << display.atk << "\n";
    os << "Def: " << display.def << "\n";
    os << "Action: " << display.currentAction << "\n";
    return os;
}
