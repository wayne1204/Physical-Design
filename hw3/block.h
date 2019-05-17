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
        _parent = NULL;
        _left = NULL;
        _right = NULL;
    }
    void setX(int x) {_x = x;}
    void setY(int y) {_y = y;}
    void setParent(Block* b) {_parent = b;}
    void setLeft(Block* b) {_left = b; }
    void setRight(Block* b) {_right = b; }
    
    string getName() {return _name; }
    int getX() {return _x;}
    int getY() {return _y;}
    int getHeight() {return _height; }
    int getWidth() {return _width;}
    Block* getParent() {return _parent;}
    Block* getLeft() {return _left; }
    Block* getRight() {return _right; }

    void showInfo();
    void rotate();
private:
    int _width;
    int _height;
    int _x;
    int _y;
    Block* _parent;
    Block* _left;    
    Block* _right;
    string _name;
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
    void setPre(Contour* c) {_pre = c;}
    void setNext(Contour* c) {_next = c;}
    void setCoord(int x1, int x2) {_min_x = x1; _max_x = x2; }
    
    int getMin() {return _min_x;}
    int getMax() {return _max_x;}
    int getY() {return _y;}
    Contour* getPre() {return _pre; };
    Contour* getNext() {return _next;}
    bool contains(int x) { return (x >= _min_x) && (x <= _max_x); }

private:
    Contour* _next;
    Contour* _pre;
    int _min_x;
    int _max_x;
    int _y;
};

class LinkedList{
public:
    LinkedList(){
        _first = NULL;
    }
    void insert(int x1, int x2, int height);
    int getMaxHeight();
    int getMaxWidth();
    void print();
    void clear();

private:
    Contour* _first;
};

#endif