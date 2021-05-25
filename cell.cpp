#include <iostream>
#include <exception>
#include <cmath>
#include "cell.h"
#include "generate.h"

Cell::Cell(char type, std::size_t pos) : type{type}, pos{pos}, ptr{nullptr}, neighbours(8, nullptr) {}

void Cell::setNeighbour(Cell *cell, std::size_t direc)
{
    neighbours.at(direc) = cell;
}

std::size_t Cell::enemyMove(int direc, std::size_t &seed)
{
    auto nghbrs = 0;
    for (auto i = 0; i < 2; ++i)
    {
        for (auto &elem : neighbours)
        {
            if (elem && elem->type == '.' && !elem->ptr)
            {
                if (!i)
                    ++nghbrs;
                else
                {
                    if (!generate(seed, 0, nghbrs - 1))
                    {
                        elem->ptr = std::move(ptr);
                        return elem->pos + 1; // THIS CANLT BE ZERO SO IT CAN BE A CHECK
                    }
                    else
                        --nghbrs;
                }
            }
        }
    }
    return 0;
}

bool Cell::enemyAttack(int direc, std::size_t &seed, std::string &action)
{
    auto player = static_cast<Character *>(neighbours.at((direc + 4) % 8)->ptr.get());
    auto enemy = static_cast<Character *>(ptr.get());
    bool doesHit = generate(seed, 0, 1);
    if (doesHit)
    {

        action += " attacks for ";
        float defense = 100 + player->def;
        float defRatio = 100 / defense;
        std::size_t damage = ceil(defRatio * enemy->atk);
        action += std::to_string(damage) + ".";
        player->hp = (damage >= player->hp) ? 0 : player->hp - damage;
        if (!player->hp)
            throw std::runtime_error("You Died in Combat. q(uit) or r(estart).\n");
        return true;
    }
    else
    {
        action += " Misses!";
        return false;
    }
}

bool Cell::playerAttack(int direc, std::string &action) // player hits, then resolve, add compass drop!
{

    if (neighbours.at(direc)->ptr)
    {
        auto player = static_cast<Character *>(ptr.get());
        auto objectType = neighbours.at(direc)->ptr.get()->getType();
        if (objectType == 'W' || objectType == 'V' || objectType == 'N' || objectType == 'T' || objectType == 'X' || objectType == 'D' || objectType == 'M')
        {
            auto enemy = static_cast<Character *>(neighbours.at(direc)->ptr.get());
            float defense = 100 + enemy->def;
            float defRatio = 100 / defense;
            std::size_t damage = ceil(defRatio * player->atk);
            enemy->hp = (damage >= enemy->hp) ? 0 : (enemy->hp - damage);
            if (enemy->hp == 0) // IF ENEMY IS KILLED REMOVE FROM VEC
            {
                if (enemy->getType() == 'M') //havent tested
                {
                    neighbours.at(direc)->ptr = std::make_unique<Gold>(Gold(4));
                    action += "Merchant slain!";
                }
                else if (enemy->getType() == 'D')
                {
                    auto dragon = static_cast<Dragon *>(enemy);
                    auto hoard = static_cast<Gold *>(neighbours.at(direc)->neighbours.at(dragon->tDirec)->ptr.get());
                    hoard->liberate();
                    std::string direction;
                    if (dragon->tDirec == 0)
                        direction = "no";
                    else if (dragon->tDirec == 1)
                        direction = "ne";
                    else if (dragon->tDirec == 2)
                        direction = "ea";
                    else if (dragon->tDirec == 3)
                        direction = "se";
                    else if (dragon->tDirec == 4)
                        direction = "so";
                    else if (dragon->tDirec == 5)
                        direction = "sw";
                    else if (dragon->tDirec == 6)
                        direction = "we";
                    else
                        direction = "nw";
                    action += "Dragon slain - loot to the " + direction + " is unguarded!";
                }
                else
                {
                    action += "Enemy slain - ";
                    //std::cout << "Basic enemy dead about to reset ptr\n";
                    neighbours.at(direc)->ptr.reset();
                    //std::cout << "reset ptr\n";
                    if (player->getType() == 'd')
                    {
                        player->gold += 2;
                        action += "keen dwarf eyes notice an extra coin!";
                    }
                    else if (player->getType() == 'o')
                        action += "your massive orc fingers fumble the loot.";
                    else
                    {
                        ++player->gold;
                        action += "you grab the loot!";
                    }
                    //std::cout << "gold attributed\n";
                }
                //std::cout << "about to exit cell playerattack\n";
                return true;
            }
            else // IF ENEMY NOT DEAD
            {
                action += "Player hits ";
                action += objectType;
                action += " for " + std::to_string(damage) + " (" + std::to_string(enemy->hp) + " HP left).";
                return true;
            }
        }
        else
        {
            action = "Swung your sword at some trinket on the ground.";
            return false;
        }
    }
    else
    {
        action = "Swung your sword at empty air.";
        return false;
    }
}

bool Cell::playerMove(int direc, std::string &action) // ADD COMPASS TO WORLD
{
    if (!neighbours.at(direc)->ptr)
    {
        neighbours.at(direc)->ptr = std::move(ptr);
        action = "Player Moved!";
    }
    else if (neighbours.at(direc)->ptr.get()->getType() == 'G' || neighbours.at(direc)->ptr.get()->getType() == 'C')
    {
        Gold *goldptr = static_cast<Gold *>(neighbours.at(direc)->ptr.get());
        if (goldptr->canTake())
        {
            action = "Player Moved: ";
            auto movee = static_cast<Character *>(ptr.get());
            if (movee->getType() == 'd')
            {
                movee->gold += 2 * goldptr->getValue();
                action += "your dwarf eyes catch a glint in the dark - gold gain x2!";
            }
            else if (movee->getType() == 'o')
            {
                if (goldptr->getValue() == 1)
                    action += "your sausage-like orc fingers fumble a gold coin.";
                else
                {
                    movee->gold += goldptr->getValue() / 2;
                    action += "your massive orc hands drop half the coins.";
                }
            }
            else
            {
                movee->gold += goldptr->getValue();
                action += "grabbed some gold!";
            }
            neighbours.at(direc)->ptr = std::move(ptr);
        }
        else
        {
            action = "Tried (and failed) to take a guarded treasure!";
            return false;
        }
    }
    else
    {
        action = "Almost moved into an occupied tile - that could've been bad.";
        return false;
    }
    return true;
}

bool Cell::use(int direc, std::string &action)
{
    if (neighbours.at(direc)->ptr)
    {
        auto potType = neighbours.at(direc)->ptr.get()->getType();
        auto player = static_cast<Player *>(ptr.get());
        if (potType == '0' || potType == '1' || potType == '2' || potType == '3' || potType == '4' || potType == '5')
        {
            if (player->getType() == 'e')
            {
                if (potType == '3')
                {
                    potType = '0';
                    action = "Elf blood changes poison to restore: ";
                }
                else if (potType == '4')
                {
                    potType = '1';
                    action = "Elf blood changes attack wound to boost: ";
                }
                else if (potType == '5')
                {
                    potType = '2';
                    action = "Elf blood changes defense wound to boost: ";
                }
            }
            if (potType == '0')
            {
                player->hp = (player->hp + 10 > player->maxHP) ? player->maxHP : player->hp + 10;
                action += "Drank soothing tonic, Health up!";
            }
            else if (potType == '1')
            {
                player->atk += 5;
                action += "Drank rage brew, Attack up!";
            }
            else if (potType == '2')
            {
                player->def += 5;
                action += "Drank stout mix, Defense up!";
            }
            else if (potType == '3')
            {
                player->hp = (player->hp < 11) ? 0 : player->hp - 10;
                action += "Drank poison, Health down!";
                if (!player->hp)
                    throw std::runtime_error("You Died to poison. q(uit) or r(estart).\n");
            }
            else if (potType == '4')
            {
                player->atk = (player->atk < 6) ? 0 : player->atk - 5;
                action += "Drank muscle relaxant, Attack down!";
            }
            else if (potType == '5')
            {
                player->def = (player->def < 6) ? 0 : player->def - 5;
                action += "Drank bone embrittlement potion, Defense down!";
            }
            neighbours.at(direc)->ptr.reset();
            return true;
        }
        else
        {
            action = "Tried to grab something that definitely wasn't a potion.";
            return false;
        }
    }
    else
    {
        action = "Reached out in the dark hoping for a potion. Hope was insufficient.";
        return false;
    }
}