#include <iostream>
#include "net.h"
#include "limits.h"
using namespace std;


int Net::getWireLength(){
    int low_x = INT_MAX, low_y = INT_MAX;
    int high_x = 0, high_y = 0;
    for(int i = 0; i < _blocks.size(); ++i){
        low_x = min(low_x, _blocks[i]->getX());
        low_y = min(low_y, _blocks[i]->getY());
        high_x = max(high_x, _blocks[i]->getX());
        high_y = max(high_y, _blocks[i]->getY());
    }
    return (high_y - low_y) + (high_x - low_x);
}

void Net::showInfo(){
    for(int i = 0; i < _blocks.size(); ++i){
        cout << _blocks[i]->getName() << " ";
    }
    cout << endl;
}