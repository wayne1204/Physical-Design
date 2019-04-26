#include <vector>
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
void Vertex::connect(int weight, Vertex* node){
    Edge* e = new Edge(weight, node);
    _edges.push_back(e);  
}

// ===============[ Router ]===============
void Router::buildGraph(int h, int v, int h_c, int v_c){
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


void Router::initSingleSource(int start_node){
    for(int i = 0; i < horizon; ++i){
        for(int j = 0; j < vertical; ++j){
            _graph[i][j]->setDistance(10000);
            _graph[i][j]->setPrevious(NULL);
        }
    }
    _graph[start_node/vertical][start_node%vertical]->setDistance(0);
}


bool Router::relax(Vertex* n1, Vertex* n2, Edge* edge){
    if(n2->getDistance() > n1->getDistance() + edge->getCost() ){
        n2->setDistance(n1->getDistance() + edge->getCost());
        n2->setPrevious(n1);
        return true;
    }
    return false;
}

void Router::Dijkstra(int sx, int sy){
    int start_node = sx * vertical + sy;
    cout << sx << sy <<endl;
    initSingleSource(start_node);
    my_queue mypq;
    for(int i = 0; i < horizon; ++i){
        for(int j = 0; j < vertical; ++ j){
            mypq.insert(_graph[i][j]);
        }
    }
    
    while(!mypq.empty()){
        Vertex* u = mypq.extract_min();
        // cout << u->getX() << " " << u->getY() << " ===================== "<< u->getDistance()  <<"\n";
        for(int i = 0; i < u->getNoEdges(); ++i){
            // cout << edge->getNode()->getX() << " " << edge->getNode()->getY() << "\n";
            Edge* e = u->getEdge(i);
            if(relax(u, e->getNode() , e)){
                mypq.decrease_key(e->getNode());
            }
        }

        // for(int i = 0; i < horizon; ++i){
        //     for(int j = 0; j < vertical; ++j){
        //         cout << _graph[i][j].getDistance() << " ";
        //     }
        //     cout <<endl;
        // }
    }
}

// layer0 : horizon layer1 vertical
void Router::traceback(int tx, int ty, int layer, int& wire_length, stringstream& ss){
    Vertex* v = _graph[tx][ty];

    if(v->getPrevious() != NULL){
        bool isHorizon = (v->getY() == v->getPrevious()->getY()) ? true : false;
        
        // via
        if(isHorizon != layer){
            ss << "(" << v->getX() << "," << v->getY() << ',' << layer+1 << ")-("
               << v->getX() << "," << v->getY() << ',' << isHorizon+1 << ")\n";
            ++wire_length;
            layer = isHorizon;
        }

        ss << '(' << tx << ',' << ty << ',' << layer+1 << ")-("
           << v->getPrevious()->getX() << ',' << v->getPrevious()->getY()
           << ',' << layer+1  << ")\n";
        updateEdge(v, v->getPrevious());
        ++wire_length;
        v = v->getPrevious();
    }
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