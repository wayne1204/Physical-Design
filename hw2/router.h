#ifndef ROUTER_H_
#define ROUTER_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
using namespace std;

// forward declaration
class my_queue;
class Vertex;
class Edge;
class Router;

class my_queue
{
public:
    Vertex* extract_min();
    void min_heapify(int index);
    void decrease_key(Vertex* v);
    void insert(Vertex* v);
    bool empty() { return A.empty(); }
private:
    vector<Vertex*> A;
};


class Vertex{
public:
    Vertex(int n1, int n2){
        _x = n1;
        _y = n2;
    }
    int getX() {return _x; }
    int getY() {return _y; }
    void connect(int weight, Vertex* node);
    void addEdge(Edge* e){_edges.push_back(e);}
    int getNoEdges() {return _edges.size(); }
    Edge* getEdge(int idx) {return _edges[idx];}
    void setDistance(double d) {_distance = d; }
    double getDistance() { return _distance; }
    bool smaller(Vertex* v) { return _distance < v->getDistance(); }
    void setPrevious(Vertex* v) {_previous = v; } 
    Vertex* getPrevious() {return _previous; }

private:
    vector<Edge*> _edges;
    Vertex* _previous;
    double _distance;
    int _x;
    int _y;
};


class Edge{
public:
    Edge(double x, Vertex* v){
        _demand = 0;
        _capacity = x; 
        _node = v;
    }

    Vertex* getNode() { return _node; }
    void adjust(int c) {_capacity = c; }
    void update() { ++_demand; }
    double getCost() { return pow(2, _demand / _capacity); }
    
private:
    double _demand;
    double _capacity;
    Vertex* _node;
};


class Router
{
public:
    void buildGraph(int h, int v, int h_c, int v_c);
    void adjustCapacity(int x1, int y1, int x2, int y2, int cap);
    void initSingleSource(int);
    bool relax(Vertex* n1, Vertex* n2, Edge* edge);
    void Dijkstra(int, int);
    void traceback(int, int, int, int&, stringstream&);
    // void writeBracket()
    void updateEdge(Vertex* v1, Vertex* v2);

private:
    int horizon;
    int vertical;
    int capacity;
    vector<vector<Vertex*> > _graph;
};

#endif