#include <iostream>
#include <exception>
#include "world.h"
#include "generate.h"
#include "objecTree.h"

bool isValidTile(char in)
{
    if (in == '.' || in == '#' || in == '+')
        return true;
    return false;
}

World::World(std::string base) : view{Display(base)}, floorNum{1}, pc{nullptr}, stairs{nullptr}, compass{nullptr}, suit{nullptr}, merchantsHostile{false}
{
    chambers.resize(5);
    for (std::size_t i = 0; i < base.length(); ++i)
    {
        auto chamber = 0;
        if (base.at(i) == '.')
        {
            if (i % 79 < 32)                   // LEFT 2 CHAMBERS
                chamber = i / 79 < 11 ? 0 : 3; // TOP OR BOTTOM LEFT CHAMBER
            else if (i / 79 > 14)              // BOTTOM RIGHT CHAMBER
                chamber = 4;
            else if (i / 79 > 8 && i % 79 < 54) // MIDDLE CHAMBER
                chamber = 2;
            else
                chamber = 1; // top right chamber
        }
        else if (base.at(i) == '#' or base.at(i) == '+')
            chamber = 5;
        else
            chamber = 6;
        curFloor.push_back(Cell(base.at(i), i));
        if (chamber < 5)
            chambers.at(chamber).push_back(curFloor.size() - 1); //stores indices to curFloor valid tiles;
    }
    int length = curFloor.size();
    for (auto i = 0; i < length; ++i)
    {
        if (isValidTile(curFloor.at(i).type))
        {
            if (i - 79 >= 0 && isValidTile(base.at(i - 79)))
                curFloor.at(i).setNeighbour(&curFloor.at(i - 79), 0); // storing ptrs to vector elems allowed because vector will no longer be resized
            if (i - 78 >= 0 && isValidTile(base.at(i - 78)))
                curFloor.at(i).setNeighbour(&curFloor.at(i - 78), 1);
            if (i + 1 < length && isValidTile(base.at(i + 1)))
                curFloor.at(i).setNeighbour(&curFloor.at(i + 1), 2);
            if (i + 80 < length && isValidTile(base.at(i + 80)))
                curFloor.at(i).setNeighbour(&curFloor.at(i + 80), 3);
            if (i + 79 < length && isValidTile(base.at(i + 79)))
                curFloor.at(i).setNeighbour(&curFloor.at(i + 79), 4);
            if (i + 78 < length && isValidTile(base.at(i + 78)))
                curFloor.at(i).setNeighbour(&curFloor.at(i + 78), 5);
            if (i - 1 >= 0 && isValidTile(base.at(i - 1)))
                curFloor.at(i).setNeighbour(&curFloor.at(i - 1), 6);
            if (i - 80 >= 0 && isValidTile(base.at(i - 80)))
                curFloor.at(i).setNeighbour(&curFloor.at(i - 80), 7);
        }
    }
}

void World::generateFloor(std::size_t seed)
{
    auto pcChamb = generate(seed, 0, 4);
    pc = &curFloor.at(chambers.at(pcChamb).at(generate(seed, 0, chambers.at(pcChamb).size() - 1)));
    view.notify(pc->pos, '@');
    // GENERATED PLAYER AND INFORMED DISPLAY
    auto stChamb = generate(seed, 0, 4);
    while (stChamb == pcChamb)
    {
        stChamb = generate(seed, 0, 4);
    }
    stairs = &curFloor.at(chambers.at(stChamb).at(generate(seed, 0, chambers.at(stChamb).size() - 1)));
    stairs->type = '/';
    view.notify(stairs->pos, '/'); // REMOVE IN FINAL, TESTING ONLY
    // GENERATED STAIRS
    auto chamber = 0;
    auto cell = 0;
    auto cellVec = chambers.at(chamber);
    for (auto i = 0; i < 20; ++i) //Generates potions then gold(and dragons)
    {
        chamber = generate(seed, 0, 4);
        cellVec = chambers.at(chamber);
        cell = generate(seed, 0, cellVec.size() - 1);
        while (curFloor.at(cellVec.at(cell)).ptr)
        {
            cell = generate(seed, 0, cellVec.size() - 1);
        }
        if (i < 10)
        {
            curFloor.at(cellVec.at(cell)).ptr = std::make_unique<GameObject>(GameObject(std::to_string(generate(seed, 0, 5))[0]));
            view.notify(curFloor.at(cellVec.at(cell)).pos, 'P'); // NOTIFY DISPLAY
        }
        else
        {
            auto goldType = generate(seed, 0, 7);
            if (goldType == 7)
                goldType = 6;
            else if (goldType > 4)
                goldType = 2;
            else
                goldType = 1;
            curFloor.at(cellVec.at(cell)).ptr = std::make_unique<Gold>(Gold(goldType));
            view.notify(curFloor.at(cellVec.at(cell)).pos, curFloor.at(cellVec.at(cell)).ptr->getType());
            if (goldType == 6) // now generate a dragon if necessary
            {
                auto direc = 0;
                auto nghbrs = 0;
                for (auto i = 0; i < 2; ++i)
                {
                    for (auto &elem : curFloor.at(cellVec.at(cell)).neighbours)
                    {
                        if (i)
                        {
                            ++direc;
                        }
                        if (elem && elem->type == '.' && !elem->ptr)
                        {
                            if (!i)
                                ++nghbrs;
                            else
                            {
                                if (!generate(seed, 0, nghbrs - 1))
                                {
                                    elem->ptr = std::make_unique<Dragon>(Dragon((direc + 4) % 8, 'D')); //NOTE DRAGONS CAN GET REMOVED BY GENERATION
                                    view.notify(elem->pos, elem->ptr->getType());
                                    break;
                                }
                                else
                                    --nghbrs;
                            }
                        }
                    }
                }
            }
        }
    }
    for (auto i = 0; i < 20; ++i) //program will hang if runs out of space in chamber, but otherwise generates all enemies
    {
        chamber = generate(seed, 0, 4);
        cellVec = chambers.at(chamber);
        cell = generate(seed, 0, cellVec.size() - 1);
        while (curFloor.at(cellVec.at(cell)).ptr)
        {
            cell = generate(seed, 0, cellVec.size() - 1);
        }
        auto enemyType = generate(seed, 0, 18);
        if (enemyType > 16)
            enemyType = 'M';
        else if (enemyType > 14)
            enemyType = 'X';
        else if (enemyType > 12)
            enemyType = 'T';
        else if (enemyType > 9)
            enemyType = 'V';
        else if (enemyType > 5)
            enemyType = 'W';
        else
            enemyType = 'N';
        curFloor.at(cellVec.at(cell)).ptr = std::make_unique<Character>(Character(enemyType));
        enemies.push_back(&curFloor.at(cellVec.at(cell)));
        view.notify(curFloor.at(cellVec.at(cell)).pos, curFloor.at(cellVec.at(cell)).ptr->getType());
    }
}

void World::setRace(char race)
{
    pc->ptr = std::make_unique<Player>(Player(race));
    auto statExtractor = static_cast<Character *>(pc->ptr.get());
    view.setRace(statExtractor->getType(), statExtractor->hp, statExtractor->atk, statExtractor->def);
    view.notify("Player has spawned.");
}

void World::clearFloor() //rn this only works for restarts, since player and floor don't carry over
{
    floorNum = 1;
    pc = nullptr;
    stairs = nullptr;
    compass = nullptr;
    suit = nullptr;
    merchantsHostile = false;
    enemies.clear();
    for (auto &elem : curFloor)
    {
        elem.ptr.reset();
    }
    view.reset();
}

bool World::attack(int direc, std::size_t &seed)
{
    std::string action;
    if (pc->neighbours.at(direc))
    {
        // std::cout << "about to resolve merchant status\n";
        if (pc->neighbours.at(direc)->ptr) // NEED TO PUT THIS BEFORE M MIGHT DIE
        {
            if (pc->neighbours.at(direc)->ptr.get()->getType() == 'M')
                merchantsHostile = true;
        }
        //std::cout << "about to call cell attack\n";
        if (pc->playerAttack(direc, action))
        {
            //std::cout << "attack successful notifying\n";
            view.notify(action); // dont need to notify about player since you dont take damage unless you resolve
            //std::cout << "done notifying, about to remove cell from enemy vector or update cell\n";
            if (!pc->neighbours.at(direc)->ptr)
            {
                for (auto &elem : enemies)
                {
                    if (elem == pc->neighbours.at(direc))
                    {
                        elem = nullptr;
                        break;
                    }
                }
                view.notify(pc->neighbours.at(direc)->pos, pc->neighbours.at(direc)->type);
            }
            else
            {
                view.notify(pc->neighbours.at(direc)->pos, pc->neighbours.at(direc)->ptr.get()->getType());
                if (pc->neighbours.at(direc)->ptr.get()->getType() == 'G')
                {
                    for (auto &elem : enemies)
                    {
                        if (elem == pc->neighbours.at(direc))
                        {
                            elem = nullptr;
                            break;
                        }
                    }
                    merchantsHostile = true;
                }
            }
            return true;
        }
        else
        {
            view.notify(action);
            return false;
        }
    }
    else
    {
        action = "Swung your sword into a wall.";
        view.notify(action);
        return false;
    }
}

bool World::move(int direc) // add stairs case, ps this does not call for enemies
{
    std::string action;
    if (pc->neighbours.at(direc))
    {
        if (pc->playerMove(direc, action))
        {
            view.notify(action);
            view.notify(pc->pos, pc->type);
            view.notify(pc->neighbours.at(direc)->pos, pc->neighbours.at(direc)->ptr.get()->getType());
            pc = pc->neighbours.at(direc);
            view.notify(static_cast<Character *>(pc->ptr.get())->gold);
            if (pc == stairs)
            {
                auto score = static_cast<Character *>(pc->ptr.get())->gold;
                if (pc->ptr.get()->getType())
                    score *= 1.5;
                throw std::logic_error(std::to_string(score));
            }
            return true;
        }
        else
        {
            view.notify(action);
            return false;
        }
    }
    else
    {
        action = "Smashed your head into a wall. Ouch.";
        view.notify(action);
        return false;
    }
}

bool World::use(int direc)
{
    std::string action;
    if (pc->neighbours.at(direc))
    {
        if (pc->use(direc, action))
        {
            view.notify(action);
            view.notify(pc->neighbours.at(direc)->pos, pc->neighbours.at(direc)->type);
            auto statExtractor = static_cast<Character *>(pc->ptr.get());
            view.notify(statExtractor->hp, statExtractor->atk, statExtractor->def);
            return true;
        }
        else
        {
            view.notify(action);
            return false;
        }
    }
    else
    {
        action = "Checked the wall for a potion. Didn't find any. Bummer.";
        view.notify(action);
        return false;
    }
}

void World::resolve(std::size_t &seed) // fix dragon attacks
{
    //std::cout << "starting to resolve\n";
    for (auto i = 0; i < 8; ++i) //this loops around pc checking for valid enemies to attack pc
    {
        auto enemy = pc->neighbours.at(i);
        if (enemy && enemy->ptr)
        {
            auto character = static_cast<Character *>(enemy->ptr.get());
            auto obj = character->getType();
            std::string action;
            if (obj == 'W' || obj == 'V' || obj == 'N' || obj == 'T' || obj == 'X')
            {
                action = " Enemy";
                enemy->enemyAttack(i, seed, action);
                view.notifyAppend(action);
                auto statExtractor = static_cast<Character *>(pc->ptr.get());
                view.notify(statExtractor->hp, statExtractor->atk, statExtractor->def);
                character->canAct = false;
            }
            else if (obj == 'D')
            {
                //std::cout << "NEXT TO DRAGON, TESTING\n";
                auto dragon = static_cast<Dragon *>(enemy->ptr.get());
                bool nextToHoard = false;
                //std::cout << "Checking if next to hoard\n";
                for (auto &object : pc->neighbours)
                {
                    if (object)
                    {
                        if (enemy->neighbours.at(dragon->tDirec))
                        {
                            if (object->pos == enemy->neighbours.at(dragon->tDirec)->pos)
                                nextToHoard = true;
                            break;
                        }
                    }
                }
                //std::cout << "Hoard Proximity checked\n";
                if (nextToHoard)
                {
                    action = " Dragon";
                    enemy->enemyAttack(i, seed, action);
                    view.notifyAppend(action);
                    auto statExtractor = static_cast<Character *>(pc->ptr.get());
                    view.notify(statExtractor->hp, statExtractor->atk, statExtractor->def);
                }
            }
            else if (obj == 'M' && merchantsHostile)
            {
                action = " Merchant";
                enemy->enemyAttack(i, seed, action);
                view.notifyAppend(action);
                auto statExtractor = static_cast<Character *>(pc->ptr.get());
                view.notify(statExtractor->hp, statExtractor->atk, statExtractor->def);
                character->canAct = false;
            }
        }
    }
    //std::cout << "about to move unresolved\n";
    for (auto &enemy : enemies) //this loops through all enemies and moves them if they haven't attacked
    {
        if (enemy)
        {
            auto enemyptr = static_cast<Character *>(enemy->ptr.get());
            if (enemyptr->canAct)
            {
                std::size_t didMove = enemy->enemyMove(0, seed);
                if (didMove)
                {
                    view.notify(enemy->pos, enemy->type);
                    view.notify(didMove - 1, curFloor.at(didMove - 1).ptr.get()->getType());
                    enemy = &curFloor.at(didMove - 1);
                }
            }
        }
    }
    //std::cout << "about to reset act bool\n";
    for (auto &enemy : enemies) //this resets all enemies to being able to act
    {
        if (enemy)
            static_cast<Character *>(enemy->ptr.get())->canAct = true;
    }
    //std::cout << "resolution complete\n";
}

void World::print()
{
    std::cout << view;
}