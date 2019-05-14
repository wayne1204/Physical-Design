#ifndef BLOCK_H
#define BLOCK_H

#include <string>
using namespace std;

class Contour;
class LinkedList;

class Block{
public:
    Block(string s, int w, int h){
        _name = s;
        _width = w;
        _height = h;
        _left = NULL;
        _right = NULL;
    }
    void setLeft(Block* b) {_left = b; }
    void setRight(Block* b) {_right = b; }
    string getName() {return _name; }
    Block* getLeft() {return _left; }
    Block* getRight() {return _right; }
    void showInfo();

private:
    string _name;
    int _width;
    int _height;
    Block* _left;    
    Block* _right;
};


class Contour{
public:
    Contour(int x1, int x2, int y){
        _min_x = x1;
        _max_x = x2;
        _y = y;
        _next = NULL;
        _pre = NULL;
    }
    int getMin() {return _min_x;}
    int getMax() {return _max_x;}
    int getY() {return _y;}
    bool within(int x) { return (x >= _min_x) || (x < _max_x); }

private:
    Contour* _next;
    Contour* _pre;
    int _min_x;
    int _max_x;
    int _y;
};

class LinkedList{
public:
    void insert(int x1, int x2, int y);
private:
    Contour* _first;
};

#endif