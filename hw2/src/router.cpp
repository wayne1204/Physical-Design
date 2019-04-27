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

    if(l < A.size() and A[l].smaller(A[index]) ){
        min = l;
    }else
        min = index;
    if(r < A.size() and A[r].smaller(A[min]) ){
        min = r;
    }
    if (min != index){
        swap(A[index], A[min]);
        min_heapify(min);
    }
}

void my_queue::insert(Vertex& v){
    A.push_back(v);
    int index = A.size()-1;
    while(index > 0 and v.smaller(A[(index-1)/2]) ){
        A[index] = A[(index-1)/2];
        index = (index-1)/2;
    }
    A[index] = v;
}

void my_queue::push(Vertex& v){
    A.push_back(v);
}

void my_queue::reserve(int size){
    A.reserve(size*2);
}

Vertex my_queue::extract_min(){
    // if(A.empty())
    //     return Vertex();
    Vertex min = A[0];
    A[0] = A.back();
    A.pop_back();
    min_heapify(0);
    return min;
}


// ===============[ Vertex ]===============
void Vertex::connect(int weight, bool b){
    _edges.push_back(Edge(weight, b));  
}

void Vertex::getPrevious(int& x, int&y){
    if(_direction == toUp){
        x = _x;
        y = _y + 1;
    }
    else if (_direction == toDown){
        x = _x;
        y = _y - 1;
    }else if(_direction == toLeft){
        x = _x - 1;
        y = _y;
    }else if(_direction == toRight){
        x = _x + 1;
        y = _y;
    }
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
        vector<Vertex> row;
        _graph.push_back(row);
        for(int j = 0; j < v; ++j){
            _graph[i].push_back(Vertex(i, j));
        }
    }

    bool upNode, downNode, leftNode, rightNode;
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < v; ++j){
            upNode   = (j != v - 1 )   ? true : false ;
            downNode = (j != 0) ? true : false ;
            leftNode = (i != 0 )   ? true : false ;
            rightNode = (i != h - 1 )? true : false ;
            
            _graph[i][j].connect(v_c, upNode);
            _graph[i][j].connect(v_c, downNode);
            _graph[i][j].connect(h_c, leftNode);
            _graph[i][j].connect(h_c, rightNode);
            // if(i == 0){
            //     _graph[i][j].connect(v_c, _graph[i+1][j]);
            // }
            // else if(i == h-1){
            //     _graph[i][j]->connect(v_c, _graph[i-1][j] );
            // }
            // else{
            //     _graph[i][j]->connect(v_c, _graph[i-1][j] );
            //     _graph[i][j]->connect(v_c, _graph[i+1][j] );
            // }

            // if(j == 0){
            //     _graph[i][j]->connect(h_c, _graph[i][j+1] );
            // }
            // else if(j == v-1){
            //     _graph[i][j]->connect(h_c, _graph[i][j-1] );
            // }
            // else{
            //     _graph[i][j]->connect(h_c, _graph[i][j-1] );
            //     _graph[i][j]->connect(h_c, _graph[i][j+1] );
            // }
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
    if(x1 == x2){
        if(y1 > y2){
            _graph[x1][y1].getEdge(toDown).adjust(cap);
            _graph[x2][y2].getEdge(toUp).adjust(cap);
        }else{
            _graph[x1][y1].getEdge(toUp).adjust(cap);
            _graph[x2][y2].getEdge(toDown).adjust(cap);
        }
    }
    else if(y1 == y2){
        if(x1 > x2){
            _graph[x1][y1].getEdge(toLeft).adjust(cap);
            _graph[x2][y2].getEdge(toRight).adjust(cap);
        }else{
            _graph[x1][y1].getEdge(toRight).adjust(cap);
            _graph[x2][y2].getEdge(toLeft).adjust(cap);
        }
    }
}


void Router::initSingleSource(int s_x, int s_y){
    for(int i = 0; i < _horizon; ++i){
        for(int j = 0; j < _vertical; ++j){
            _graph[i][j].setDistance(10000);
            _graph[i][j].setPrevious(undefined);
        }
    }
    _graph[s_x][s_y].setDistance(0);
}

// n1 + dirction(up, dwon, l ,r) = n2
bool Router::relax(Vertex& n1, int direction){
    short reverse_direction = (direction == toUp || direction == toLeft) ?
                                       direction + 1 : direction - 1;
    Vertex n2 = _graph[n1.getX()+_movement[direction*2]][n1.getY() + _movement[direction*2+1]];
    if(n2.getDistance() > n1.getDistance() + n1.getEdge(direction).getCost() ){
        n2.setDistance(n1.getDistance() + n1.getEdge(direction).getCost());
        n2.setPrevious(reverse_direction);
        return true;
    }
    return false;
}

void Router::Dijkstra(int sx, int sy, int tx, int ty){
    initSingleSource(sx, sy);
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

    // cout << "\nbang" << sx << " " << sy << " " << tx << " " << ty <<endl;
    while(!mypq.empty()){
        Vertex u = mypq.extract_min();
        // cout << setw(8) << mypq.size() << '\r';
        for(int i = 0; i < 4; ++i){
            if(!u.getEdge(i).exist()){
                continue;
            }
            if(relax(u, i)){
                mypq.insert(_graph[u.getX()+_movement[i*2]][u.getY()+_movement[i*2]]);
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
    Vertex v = _graph[tx][ty];
    int x1, y1;
    while(v.getPreDirection() != undefined){
        bool isVertical = (v.getPreDirection() == toUp || v.getPreDirection() == toDown);
        // via
        if(isVertical != layer){
            if(layer == 0)
                writeConnect(v, layer+1, ascend, ss);
            else
                writeConnect(v, layer+1, descend, ss);
            ++wire_length;
            layer = isVertical;
        }
        writeConnect(v, layer+1, v.getPreDirection(), ss);
        updateEdge(v, v.getPreDirection());
        ++wire_length;
        
        v.getPrevious(x1, y1);
        v = _graph[x1][y1];
    }
    // cout << v->getX() << v->getY()  << endl;
    if(layer){
        writeConnect(v, layer+1, descend, ss);
        ++wire_length;
    }
}

// if there is already net skip it
void Router::writeConnect(Vertex& v1, int layer, int direction, stringstream& ss){
    int key = direction*_horizon*_vertical + v1.getX()*_vertical+ v1.getY();
    if(_existing_path.find(key) != _existing_path.end())
        return;
    _existing_path.insert(key);
    if(direction < 4){
        ss << "(" << _lower_left_x + floor(_width * (v1.getX() + 0.5))
        << "," << _lower_left_y + floor(_height * (v1.getY() + 0.5)) 
        << ',' << layer
        << ")-(" << _lower_left_x + floor(_width * (v1.getX()+ _movement[direction*2] + 0.5))
        << "," << _lower_left_y + floor(_height * (v1.getY() + _movement[direction*2+1] +0.5))  
        << ',' << layer << ")\n";
    }
    else{
        ss << "(" << _lower_left_x + floor(_width * (v1.getX() + 0.5))
        << "," << _lower_left_y + floor(_height * (v1.getY() + 0.5)) 
        << ',' << layer
        << ")-(" << _lower_left_x + floor(_width * (v1.getX()+ 0.5))
        << "," << _lower_left_y + floor(_height * (v1.getY()+0.5))  
        << ',' << layer << ")\n";
    }
}


void Router::updateEdge(Vertex& v1, short direction){
    int x, y;
    v1.getPrevious(x, y);
    Vertex v2 = _graph[x][y];
    short reverse_direction = (direction == toUp || direction == toLeft) ?
                                       direction + 1 : direction - 1;

    v1.getEdge(direction).update();
    v1.getEdge(reverse_direction).update();   
}