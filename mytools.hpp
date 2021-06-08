#ifndef MYTOOLS
#define MYTOOLS

#include <random>

namespace tool
{
    int randInt(int min, int max)
    {
        std::random_device rd;
        std::default_random_engine e(rd());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(e);
    }

    int square(int a)
    {
        return a * a;
    }

    int abso(int a)
    {
        return a < 0 ? -a : a;
    }
}

#endif
