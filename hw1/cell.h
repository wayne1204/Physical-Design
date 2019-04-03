#ifndef CELL_H
#define CELL_H

#include <vector>
#include "net.h"

class Cell{
public:
    void addNet(Net* n) {connected_nets_.push_back(n);}

private:
    vector<Net*> connected_nets_;
};

#endif