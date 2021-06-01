#ifndef INTGEN
#define INTGEN

#include <random>

int randInt(int min, int max)
{
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(e);
}

#endif
