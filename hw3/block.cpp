#include <iostream>
#include "block.h"

void Block::showInfo(){
    printf("%s: (%d, %d)\n", _name.c_str(), _width, _height);
}

void LinkedList::insert(int x1, int x2, int y){
    if(_first == NULL){
        _first = new Contour(x1, x2, y);
        return;
    }
    Contour* low, *high;
    while(_first != NULL){
        if(_first->within(x1))
            low = _first;
        if(_first->within(x2)){
            high = _first;
            break;
        }
    }

    // case 1: same block
    if(low == high){
        Contour* left = new Contour(low->getMin(), x1, low->getY());
    }
}