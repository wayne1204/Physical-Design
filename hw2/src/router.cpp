#include <vector>
#include <math.h>
#include <queue>
#include <iomanip>
#include "router.h"

// ===============[ my_queue]===============


void my_queue::min_heapify(int index){
    int l = index * 2 + 1;
    int r = index * 2 + 2;
    int min;

    if(l < A.size() and A[l]->smaller(A[index]) ){
        min = l;
    }else
        min = index;
    if(r < A.size() and A[r]->smaller(A[min]) ){
        min = r;
    }
    if (min != index){
        swap(A[index], A[min]);
        min_heapify(min);
    }
}

void my_queue::insert(Vertex* v){
    A.push_back(v);
    int index = A.size()-1;
    while(index > 0 and v->smaller(A[(index-1)/2]) ){
        A[index] = A[(index-1)/2];
        index = (index-1)/2;
    }
    A[index] = v;
}

void my_queue::push(Vertex* v){
    A.push_back(v);
}

void my_queue::reserve(int size){
    A.reserve(size);
}

Vertex* my_queue::extract_min(){
    if(A.empty())
        return NULL;
    Vertex* min = A[0];
    A[0] = A.back();
    A.pop_back();
    min_heapify(0);

    return min;
}


void my_queue::decrease_key(Vertex* v){
    for(int i = 0; i < A.size(); ++i){
        if(v == A[i]){
            while(i > 0 and v->smaller(A[(i-1)/2]) ){
                A[i] = A[(i-1)/2];
                i = (i-1)/2;
            }
            A[i] = v;
        }
    }
}

// ===============[ Vertex ]===============
int Vertex::getX(){
    return _x;
}

int Vertex::getY(){
    return _y;
}

void Vertex::connect(int weight, Vertex* node){
    Edge* e = new Edge(weight, node);
    _edges.push_back(e);  
}


// ===============[ Router ]===============
void Router::init(){
    _existing_path.clear();
}

void Router::buildGraph(int h, int v, int h_c, int v_c){
    _horizon = h;
    _vertical = v;
    _h_capacity = h_c;
    _v_capacity = v_c;
    for(int i = 0; i < h; ++i){
        vector<Vertex*> row;
        _graph.push_back(row);
        for(int j = 0; j < v; ++j){
            _graph[i].push_back(new Vertex(i, j));
        }
    }

    Vertex* upNode, *downNode, *leftNode, *rightNode;
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < v; ++j){
            // upNode   = (i != 0 )   ? _graph[i+1][j] : NULL ;
            // downNode = (i != h -1) ? _graph[i-1][j] : NULL ;
            // leftNode = (j != 0 )   ? _graph[i][j-1] : NULL ;
            // rightNode = (j != v-1 )? _graph[i][j+1] : NULL ;

            // _graph[i][j]->connect(v_c, upNode);
            // _graph[i][j]->connect(v_c, downNode);
            // _graph[i][j]->connect(v_c, downNode);
            // _graph[i][j]->connect(v_c, downNode);
            if(i == 0){
                _graph[i][j]->connect(v_c, _graph[i+1][j] );
            }
            else if(i == h-1){
                _graph[i][j]->connect(v_c, _graph[i-1][j] );
            }
            else{
                _graph[i][j]->connect(v_c, _graph[i-1][j] );
                _graph[i][j]->connect(v_c, _graph[i+1][j] );
            }

            if(j == 0){
                _graph[i][j]->connect(h_c, _graph[i][j+1] );
            }
            else if(j == v-1){
                _graph[i][j]->connect(h_c, _graph[i][j-1] );
            }
            else{
                _graph[i][j]->connect(h_c, _graph[i][j-1] );
                _graph[i][j]->connect(h_c, _graph[i][j+1] );
            }
        }
    }
}

void Router::setCoordinate(int low_x, int low_y, int w, int h){
    _lower_left_x = low_x;
    _lower_left_y = low_y;
    _width = w;
    _height = h;
}

void Router::adjustCapacity(int x1, int y1, int x2, int y2, int cap){
    Vertex* v1 = _graph[x1][y1];
    Vertex* v2 = _graph[x2][y2];
    for(int i = 0; i < v1->getNoEdges(); ++i){
        if(v1->getEdge(i)->getNode() == v2){
            v1->getEdge(i)->adjust(cap);
            break;
        }
    }

    for(int i = 0; i < v2->getNoEdges(); ++i){
        if(v2->getEdge(i)->getNode() == v1){
            v2->getEdge(i)->adjust(cap);
            break;
        }
    }
}


void Router::initSingleSource(int s_x, int s_y){
    for(int i = 0; i < _horizon; ++i){
        for(int j = 0; j < _vertical; ++j){
            _graph[i][j]->setDistance(10000);
            _graph[i][j]->setPrevious(NULL);
        }
    }
    _graph[s_x][s_y]->setDistance(0);
}


bool Router::relax(Vertex* n1, Vertex* n2, Edge* edge){
    if(n2->getDistance() > n1->getDistance() + edge->getCost() ){
        n2->setDistance(n1->getDistance() + edge->getCost());
        n2->setPrevious(n1);
        return true;
    }
    return false;
}

void Router::Dijkstra(int sx, int sy, int tx, int ty){
    initSingleSource(sx, sy);
    // priority_queue<Vertex*, vector<Vertex*>, mycomparision> mypq ;
    my_queue mypq;
    mypq.reserve(_horizon * _vertical);
    for(int i = 0; i < _horizon; ++i){
        for(int j = 0; j < _vertical; ++ j){
            if(i == sx && j == sy)
                mypq.insert(_graph[i][j]);
            else
                mypq.push(_graph[i][j]);
        }
    }
    bool stop = false;
    cout << "\nbang" << sx << " " << sy << " " << tx << " " << ty <<endl;
    while(!mypq.empty() && !stop){
        Vertex* u = mypq.extract_min();
        cout << setw(8) << mypq.size() << '\r';
        for(int i = 0; i < u->getNoEdges(); ++i){
            Edge* e = u->getEdge(i);
            if(relax(u, e->getNode() , e)){
                // mypq.decrease_key(e->getNode());
                mypq.insert(e->getNode());
            }
        }

        // for(int j = _vertical - 1; j >= 0; --j){
        //     for(int i = 0; i < _horizon; ++i){
        //         cout << _graph[i][j]->getDistance() << " ";
        //     }
        //     cout <<endl;
        // }
        // cout << endl;
    }
}

// layer0 : horizon layer1 vertical
// isVertical 0 -> H  1 -> V
void Router::traceback(int tx, int ty, int layer, int& wire_length, stringstream& ss){
    Vertex* v = _graph[tx][ty];
    while(v->getPrevious() != NULL){
        bool isVertical = (v->getX() == v->getPrevious()->getX()) ? true : false;
        // via
        if(isVertical != layer){
            writeConnect(v, v, layer+1, isVertical+1, ss);
            ++wire_length;
            layer = isVertical;
        }
        writeConnect(v, v->getPrevious(), layer+1, layer+1, ss);
        updateEdge(v, v->getPrevious());
        ++wire_length;
        v = v->getPrevious();
    }
    // cout << v->getX() << v->getY()  << endl;
    if(layer){
        writeConnect(v, v, layer+1, layer, ss);
        ++wire_length;
    }
}

// if there is already net skip it
void Router::writeConnect(Vertex* v1, Vertex* v2, int l1, int l2, stringstream& ss){
    int key = getDirection(v1, v2, l1, l2)*_horizon*_vertical + v1->getX()*_vertical+ v1->getY();
    if(_existing_path.find(key) != _existing_path.end())
        return;
    _existing_path.insert(key);

    ss << "(" << _lower_left_x + floor(_width * (v1->getX() + 0.5))
       << "," << _lower_left_y + floor(_height * (v1->getY() + 0.5)) 
       << ',' << l1 
       << ")-(" << _lower_left_x + floor(_width * (v2->getX() + 0.5))
       << "," << _lower_left_y + floor(_height * (v2->getY() + 0.5))  
       << ',' << l2 << ")\n";
}

int Router::getDirection(Vertex* v1, Vertex* v2, int l1, int l2){
    enum direction{
        up, down, north, south, west, east,
    };
    if(l1 != l2){
        if(l1 - l2 > 0)
            return up;
        else
            return down;
    }
    else if(v1->getX() != v2->getX()){
        if(v1->getX() > v2->getX())
            return west;
        else 
            return east;
    }
    else if(v1->getY() != v2->getY()){
        if(v1->getY() > v2->getY())
            return south;
        else 
            return north;
    }
    // should never reach here
    return -1;
}

void Router::updateEdge(Vertex* v1, Vertex* v2){
    for(int i = 0; i < v1->getNoEdges(); ++i){
        Edge* edge = v1->getEdge(i);
        if(edge->getNode() == v2){
            edge->update();
            break;
        }
    }

    for(int i = 0; i < v2->getNoEdges(); ++i){
        Edge* edge = v2->getEdge(i);
        if(edge->getNode() == v1){
            edge->update();
            break;
        }
    }
}