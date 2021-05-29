#ifndef CELL
#define CELL

#include <vector>

struct Cell
{
    bool hasNo = false;
    bool hasEa = false;
    bool hasSo = false;
    bool hasWe = false;
};

struct DisplayCell
{
    char type = '.';
    char overwrite = '.';
};

#endif
