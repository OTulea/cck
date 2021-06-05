#ifndef CELL
#define CELL

#include <vector>

struct Cell
{
    char type = '.';
    char contained = type;
    bool seen = false;
    bool visible = false;
    bool hasNo = true;
    bool hasEa = true;
    bool hasSo = true;
    bool hasWe = true;
};

#endif
