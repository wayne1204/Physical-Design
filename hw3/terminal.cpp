#include <iostream>
#include "terminal.h"

void Terminal::showInfo(){
    printf("%s: (%d, %d)\n", _name.c_str(), _x, _y);
}