#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
using namespace std;

class Terminal{
public:
    Terminal(string s, int x, int y){
        _name = s;
        _x = x;
        _y = y;
    }
    void showInfo();
    string getName() {return _name;}
private:
    string _name;
    int _x;
    int _y;
};

#endif