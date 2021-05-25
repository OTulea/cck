#include <random>
#include <chrono>
#include "generate.h"

int generate(int min, int max)
{
    auto a = std::chrono::system_clock::now();
    std::mt19937 gen{a};
    std::uniform_int_distribution<int> dist{min, max};
    return dist(gen);
}