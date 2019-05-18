#include <iostream>
#include "block.h"

void Block::showInfo(){
    printf("%s: (%d, %d) w:%d, h:%d \n", _name.c_str(), _x, _y, _width, _height);
}

void Block::rotate(){
    int tmp = _height;
    _height = _width;
    _width = tmp;
}

int LinkedList::insert(int x1, int x2, int h){
    // cout << x1 << " "<< x2 << " "<< h <<endl;
    if(_first == NULL){
        _first = new Contour(x1, x2, h);
        return 0;
    }

    Contour* left_adj = NULL, *right_adj = NULL;
    Contour* cur = _first, *end = NULL;
    while(cur != NULL){
        if(cur->contains(x1)){
            left_adj = cur;
            if(x1 == left_adj->getMin()){
                left_adj = left_adj->getPre();
            }
        }
        if(cur->contains(x2)){
            right_adj = cur;
            end = cur;
            if(x2 == right_adj->getMax()){
                right_adj = right_adj->getNext();
            }
            break;
        }
        cur = cur->getNext();
    }

    // find y coordinates by find covered blocks
    int y = 0;
    // if(end != NULL){
        Contour* start = left_adj; //, *end = right_adj;
        if(left_adj == NULL){
            start = _first;
        }
        start = (x1 == start->getMax()) ? start->getNext() : start;
        while(start != NULL){
            y = max(y, start->getY());
            if(start == end)
                break;
            start = start->getNext();
        }
    // }
    // cout << y <<endl;

    // case 0: all null
    if(left_adj == NULL && right_adj == NULL){
        _first = new Contour(x1, x2, y+h);
    }
    // case 1: same block
    else if(left_adj == right_adj){
        y = left_adj->getY();
        cout << "case1" <<endl;
        Contour* outside = left_adj->getNext();
        int orignal_x2 = left_adj->getMax();

        Contour* right = left_adj;
        right->setCoord(x2, left_adj->getMax());
        left_adj->setCoord(left_adj->getMin(), x1); 
        Contour* middle = new Contour(x1, x2, y + h);

        left_adj->setNext(middle);
        middle->setNext(right);
        right->setPre(middle);
        middle->setPre(left_adj);
    }
    // case 2: select two boundary blocks
    else{
        // cout << "case2" <<endl;
        Contour* middle = new Contour(x1, x2, y + h);
        middle->setPre(left_adj);
        middle->setNext(right_adj);

        if(x1 == 0){
            _first = middle;
        }
        if(left_adj != NULL){
            left_adj->setCoord(left_adj->getMin(), x1);
            left_adj->setNext(middle);
        }
        if(right_adj != NULL){
            right_adj->setCoord(x2, right_adj->getMax());
            right_adj->setPre(middle);
        }
    }
    return y;
}

int getY(){

}
int LinkedList::getMaxHeight(){
    int ret = 0;
    Contour* cur = _first;
    while(cur != NULL){
        ret = max(ret, cur->getY());
        cur = cur->getNext();
    }
    return ret;
}

int LinkedList::getMaxWidth(){
    int ret = 0;
    Contour* cur = _first;
    while(cur != NULL){
        ret = max(ret, cur->getMax());
        cur = cur->getNext();
    }
    return ret;
}

void LinkedList::print(){
    Contour* cur = _first;
    while(cur != NULL){
        printf("(%d, %d) height: %d \n", cur->getMin(), cur->getMax(), cur->getY());
        cur = cur->getNext();
    }
}


void LinkedList::clear(){
    while(_first != NULL){
        Contour* tmp = _first;
        _first = _first->getNext();
        delete tmp;
    }
}
