#ifndef ROUTER_H_
#define ROUTER_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <unordered_set>
#include <queue>
using namespace std;


// forward declaration
class my_queue;
class Vertex;
class Edge;
class Router;
class mycomparision;

enum direction
{
    toUp,
    toDown,
    toLeft,
    toRight,
    ascend,
    descend,
    undefined,
};

class my_queue
{
public:
    Vertex extract_min();
    void min_heapify(int index);
    void decrease_key(Vertex& v);
    void insert(Vertex& v);
    void reserve(int size);
    void push(Vertex& v);

    bool empty() { return A.empty(); }
    int size() {return A.size(); }
private:
    vector<Vertex> A;
};


class Vertex{
public:
    Vertex(short n1, short n2){
        _x = n1;
        _y = n2;
    }
    int getX() {return _x;}
    int getY() {return _y;}
    void connect(int weight, bool b);
    Edge& getEdge(int idx) {return _edges[idx];}
    void setDistance(double d) {_distance = d; }
    double getDistance() { return _distance; }
    bool smaller(Vertex& v) { return _distance < v.getDistance(); }
    void setPrevious(short v) {_direction = v; } 
    void getPrevious(int&, int&);
    short getPreDirection() {return _direction; }

private:
    vector<Edge> _edges;
    double _distance;
    short _x;
    short _y;
    short _direction;
};


class Edge{
public:
    Edge(double x, bool b){
        _demand = 0;
        _capacity = x; 
        _exist = b;
    }

    bool exist() {return _exist;}
    void adjust(int c) {_capacity = c; }
    void update() { ++_demand; }
    double getCost() { return pow(2, _demand / _capacity); }
    
private:
    bool _exist;
    double _demand;
    double _capacity;
};


class Router
{
public:
    Router(){
        _movement.push_back(0);
        _movement.push_back(1);
        _movement.push_back(0);
        _movement.push_back(-1);
        _movement.push_back(-1);
        _movement.push_back(0);
        _movement.push_back(1);
        _movement.push_back(0);
    }
    void init();
    void buildGraph(int h, int v, int h_c, int v_c);
    void setCoordinate(int , int, int, int);
    void adjustCapacity(int x1, int y1, int x2, int y2, int cap);
    void initSingleSource(int, int);
    bool relax(Vertex& n1, int);
    void Dijkstra(int, int, int, int);
    void traceback(int, int, int, int&, stringstream&);
    void writeConnect(Vertex&, int, int, stringstream&);
    void updateEdge(Vertex& v1, short direction);

private:
    // coored
    int _lower_left_x;
    int _lower_left_y;
    int _width;
    int _height;

    int _horizon;
    int _vertical;
    int _h_capacity;
    int _v_capacity;
    vector<vector<Vertex> > _graph;
    vector<int> _movement;
    unordered_set<int> _existing_path;
};

class mycomparision
{
public:
    bool operator() ( Vertex* v1,  Vertex* v2){
        return(v1->getDistance() < v2->getDistance());
    }
};

#endif