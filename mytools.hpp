#ifndef MYTOOLS
#define MYTOOLS

#include <random>

inline int randInt(int min, int max) // needs to be inlined to be included in multiple places?
{
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(e);
}

inline int square(int a)
{
    return a * a;
}

inline int abso(int a)
{
    return a < 0 ? -a : a;
}

inline int max(int a, int b)
{
    return a <= b ? b : a;
}

inline int min(int a, int b)
{
    return a <= b ? a : b;
}
#endif
