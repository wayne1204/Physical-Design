#include <iostream>
#include <fstream>

#include "floorplanner.h"
#include "block.h"
using namespace std;

void Floorplanner::parseBlocks(const char* file){
    fstream fs(file, ios::in);
    string token, name;
    int n_block, n_termianl;
    bool readBlock = true;


    while(fs >> token){
        cout << token <<endl;
        if(token == "Outline:"){
            fs >> token;
            int w = atoi(token.c_str());
            fs >> token;
            int h = atoi(token.c_str());
            setCoordinates(w, h);
        }
        else if(token == "NumBlocks:"){
            fs >> token;
            n_block = atoi(token.c_str());
        }
        else if(token == "NumTerminals:"){
            fs >> token;
            n_termianl = atoi(token.c_str());
            for(int i = 0; i < n_block; ++i){
                fs >> token;
                name = token;
                
                fs >> token;
                int w = atoi(token.c_str());
                fs >> token;
                int h = atoi(token.c_str());
                Block* block = new Block(name, w, h);
                _blocks_V.push_back(block);
                _blocks.emplace(name, block);
            }
            for(int i = 0; i < n_termianl; ++i){
                fs >> token;
                name = token;
                fs >> token >> token;
                int x = atoi(token.c_str());
                fs >> token;
                int y = atoi(token.c_str());
                Terminal* ter = new Terminal(name, x, y);
                _terminals.emplace(name, ter);
            }
        }
        else{
            cout <<"wrong!\n";
        }
    }
    cout << "parse blocks " << endl;
}

void Floorplanner::parseNets(const char* file){
    fstream fs(file, ios::in);
    string token;
    int n_net, n_degree;

    fs >> token >> token;
    n_net = atoi(token.c_str());
    cout << n_net<<endl;
    for(int i = 0; i < n_net; ++i){
        fs >> token >> token;
        n_degree = atoi(token.c_str());
        Net* n = new Net(n_degree);

        for(int j = 0; j < n_degree; ++j){
            fs >> token;
            auto it = _blocks.find(token);
            if(it == _blocks.end()){
                auto it = _terminals.find(token);
                n->addTerminal(it->second);
            }
            else{
                n->addBlock(it->second);
            }
        }
        _nets.push_back(n);
    }
    cout << "parse nets" <<endl;
}

Block* Floorplanner::buildTree(Block* root, int index){
    if(index < _blocks_V.size()){
        root = _blocks_V[index];
        cout << index << endl;
        root->setLeft(buildTree(root->getLeft() , (index+1)*2-1));
        root->setRight(buildTree(root->getRight(), (index+1)*2));
    }
    return root;
}

void Floorplanner::genSolution(){
    int times = 10;
    while(times > 0){
            
        --times;
    }
}
void Floorplanner::simulateAnnealing(){
    _root = buildTree(_root, 0);
    double temp = 1000;
    double gamma = 0.999;
    int threshold = 1;

    while (temp > threshold)
    {
        
    }
}

void Floorplanner::showStatus(){
    printf("===== [info ] =====\n");
    printf("[alpha]: %f \n",  _alpha);
    printf("[coord]: (%d, %d) \n", _outline_w, _outline_h);
    printf("[Blocks] nums: %d \n", _blocks.size());
    // for(auto it = _blocks.begin(); it != _blocks.end(); ++it){
    //     it->second->showInfo();
    // }
    printf("[Terminals] nums: %d \n", _terminals.size());
    // for(auto it = _terminals.begin(); it != _terminals.end(); ++it){
    //     it->second->showInfo();
    // }
    printf("[Nets] nums: %d\n", _nets.size());
    // for(int i = 0; i < _nets.size(); ++i){
    //     _nets[i]->showInfo();
    // }
}