#ifndef NET_H
#define NET_H

#include <vector>
#include "cell.h"

class Net{

public:
    void addCell(Cell* c) {connected_cells_.push_back(c);}

private:
    vector<Cell*> connected_cells_;
};

#endif

