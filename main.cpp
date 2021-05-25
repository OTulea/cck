#include <iostream>
#include <fstream>
#include <exception>
#include "controller.h"

bool isValidDirec(const std::string input)
{
    if (input == "no" || input == "ne" || input == "ea" || input == "se" || input == "so" || input == "sw" || input == "we" || input == "nw")
        return true;
    return false;
}

std::size_t direcConvert(const std::string input)
{
    if (input == "no")
        return 0;
    else if (input == "ne")
        return 1;
    else if (input == "ea")
        return 2;
    else if (input == "se")
        return 3;
    else if (input == "so")
        return 4;
    else if (input == "sw")
        return 5;
    else if (input == "we")
        return 6;
    else
        return 7;
}

void clearInput() // this assumes no errors in reading, might want to update it
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input, please try again.\n";
}

std::string fileRead(const std::string input)
{
    std::ifstream file(input);
    char in;
    std::string str;
    while (file >> std::noskipws >> in)
    {
        if (in == ' ' || !isspace(in))
            str += in;
    }
    return str;
}

int main(int argc, char *argv[])
{
    std::size_t baseSeed = 67; //make this based on system time for unspecified seed
    std::string inputFile = "input.txt"; // parser needs work
    Controller game(fileRead(inputFile));
    std::string cmd, cmd2;
    auto restart = false;
    while (true)
    {
        auto isOver = false;
        std::cout << "Enter q(uit), h(uman), d(warf), e(lf) or o(rc)\n";
        while (std::cin >> cmd && cmd != "h" && cmd != "d" && cmd != "e" && cmd != "o")
        {
            if (cmd == "q")
                return 0;
            clearInput();
        }
        game.start(cmd[0], baseSeed, restart);
        auto seed = baseSeed;
        while (!restart && std::cin >> cmd)
        {
            if (cmd == "q")
                return 0;
            else if (cmd == "r") 
                restart = true;
            else if (!isOver && cmd == "a" && std::cin >> cmd2 && isValidDirec(cmd2))
            {
                try
                {
                    game.interact('a', direcConvert(cmd2), seed);
                }
                catch (std::runtime_error &e)
                {
                    std::cout << e.what();
                    isOver = true;
                }
                catch (std::logic_error &e) {
                    std::cout << "You Win! Score: " << e.what() << ". q(uit) or r(estart)";
                    isOver = true;
                }
            }
            else if (!isOver && cmd == "u" && std::cin >> cmd2 && isValidDirec(cmd2))
            {
                try
                {
                    game.interact('u', direcConvert(cmd2), seed);
                }
                catch (std::runtime_error &e)
                {
                    std::cout << e.what();
                    isOver = true;
                }
                catch (std::logic_error &e) {
                    std::cout << "You Win! Score: " << e.what() << ". q(uit) or r(estart)";
                    isOver = true;
                }

            }
            else if (!isOver && isValidDirec(cmd))
            {
                try
                {
                    game.interact('m', direcConvert(cmd), seed);
                }
                catch (std::runtime_error &e)
                {
                    std::cout << e.what();
                    isOver = true;
                }
                catch (std::logic_error &e) {
                    std::cout << "You Win! Score: " << e.what() << ". q(uit) or r(estart)";
                    isOver = true;
                }
            }
            else
                clearInput();
        }
    }
}