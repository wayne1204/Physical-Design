#ifndef NET_H
#define NET_H

#include <string>
#include <vector>
#include "block.h"
#include "terminal.h"
using namespace std;


class Net{
public:
    Net(int n){
        _blocks.reserve(n);
    }
    void addBlock(Block* b){_blocks.push_back(b); }
    void addTerminal(Terminal* b){_terminals.push_back(b); }
    int getWireLength();
    void showInfo();

private:
    vector<Block*> _blocks;
    vector<Terminal*> _terminals;
};

#endif