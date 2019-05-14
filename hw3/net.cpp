#include <iostream>
#include "net.h"
using namespace std;


int Net::getWireLength(){
    int low_x = INT_MAX, low_y = INT_MAX;
    int high_x = 0, high_y = 0;
    return 0;
}

void Net::showInfo(){
    cout << "B: ";
    for(int i = 0; i < _blocks.size(); ++i){
        cout << _blocks[i]->getName() << " ";
    }
    cout << endl << "T: ";
    for(int i = 0; i < _terminals.size(); ++i){
        cout << _terminals[i]->getName() << " ";
    }
    cout << endl;
}