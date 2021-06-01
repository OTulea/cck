#ifndef CELL
#define CELL

#include <vector>

struct Cell
{
    char type = '.';
    char contained = type;
    bool seen = false;
    bool visible = false;
    bool hasNo = false;
    bool hasEa = false;
    bool hasSo = false;
    bool hasWe = false;
};

#endif
