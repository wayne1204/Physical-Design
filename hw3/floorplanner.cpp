#include <iostream>
#include <fstream>
#include <stack>

#include <stdlib.h>
#include "floorplanner.h"
#include "block.h"
using namespace std;

void Floorplanner::parseBlocks(const char* file){
    fstream fs(file, ios::in);
    string token, name;
    int n_block, n_termianl;
    bool readBlock = true;
    int total_block_size = 0;

    cout << "... parse blocks " << endl;
    while(fs >> token){
        if(token == "Outline:"){
            fs >> token;
            _outline_w = atoi(token.c_str());
            fs >> token;
            _outline_h = atoi(token.c_str());
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
                total_block_size += (w * h);
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
    _total_sizes = total_block_size;
}

void Floorplanner::parseNets(const char* file){
    fstream fs(file, ios::in);
    string token;
    int n_net, n_degree;

    cout << "... parse nets " << endl;
    fs >> token >> token;
    n_net = atoi(token.c_str());
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
}

Block* Floorplanner::buildTree(Block* root, int index){
    if(index < _blocks_V.size()){
        // cout << index << " " << _blocks_V[index]->getName() << endl;
        root = _blocks_V[index];
        root->setLeft(buildTree(root->getLeft() , (index+1)*2-1));
        if(root->getLeft() != NULL)
            root->getLeft()->setParent(root);
        root->setRight(buildTree(root->getRight(), (index+1)*2));
        if(root->getRight() != NULL)
            root->getRight()->setParent(root);
    }
    return root;
}

void Floorplanner::deleteNode(Block* block){
    // case 1. no child
    if(block->getLeft() == NULL && block->getRight() == NULL){
        // TODO
        if(block == block->getParent()->getLeft()){
            block->getParent()->setLeft(NULL);
        }else{
            block->getParent()->setRight(NULL);
        }
        // block->setParent(NULL);
    }
    // case 2. one child
    else if (block->getLeft() == NULL){
        if(block == block->getParent()->getLeft()){
            block->getParent()->setLeft(block->getRight());
        }else{
            block->getParent()->setRight(block->getRight());
        }
        block->getRight()->setParent(block->getParent());
    }   
    else if(block->getRight() == NULL){
        if(block == block->getParent()->getLeft()){
            block->getParent()->setLeft(block->getLeft());
        }else{
            block->getParent()->setRight(block->getLeft());
        }
        block->getLeft()->setParent(block->getParent());
    }
    // case 3. two child
    else{
        // while not leaf
        Block* replace;
        while(block->getLeft() != NULL || block->getRight() != NULL){
            if(block->getLeft() != NULL)
                replace = block->getLeft();
            else
                replace = block->getRight();
            if(block == block->getParent()->getLeft()){
                replace->setParent(block->getParent());
                block->getParent()->setLeft(replace);
            }else{
                replace->setParent(block->getParent());
                block->getParent()->setRight(replace);
            }
            block = replace;
        }
    }
}

void Floorplanner::insertNode(Block* block, Block* insert, bool isLeft){
    // TODO
    Block* child = isLeft ? block->getLeft() : block->getRight();
    if(isLeft){
        block->setLeft(insert);
        insert->setParent(block);
        if(child != NULL){
            child->setParent(insert);
            insert->setLeft(child);
        }
    }else{
        block->setRight(insert);
        insert->setParent(block);
        if(child != NULL){
            child->setParent(insert);
            insert->setRight(child);   
        }
    }
}

void Floorplanner::swapNode(Block* block1, Block* block2){
    // TODO
    Block* parent_1 = block1->getParent();
    Block* left_1 = block1->getLeft();
    Block* right_1 = block1->getRight();

    block1->setParent(block2->getParent());
    block1->setLeft(block2->getLeft());
    block1->setRight(block2->getRight());
    block2->setParent(parent_1);
    block2->setLeft(left_1);
    block2->setRight(right_1);
}

void Floorplanner::perturb(){
    int len = _blocks.size();
    int action = rand() % 3;
    int block_1 = rand() % len;
    int block_2 = rand() % len;
    while(block_2 == block_1){
        block_2 = rand() % len;
    }
    if(action == 0){
        // rotate 
        cout << "rotate \n";
        _blocks_V[block_1]->rotate();
    }
    else if(action == 1){
        // delete & insert
        cout << "delete " << _blocks_V[block_1]->getName() << 
             " & insert below " << _blocks_V[block_2]->getName() << "\n";
        deleteNode(_blocks_V[block_1]);
        insertNode(_blocks_V[block_2], _blocks_V[block_1], rand() % 2);
    }
    else{
        // swap 
        cout << "swap!" <<endl;
    }

}

bool Floorplanner::evaluateFeasible(int& width, int& height){
    stack<Block*> stk;
    LinkedList mylist;
    stk.push(_root);
    while(!stk.empty()){
        Block* blk = stk.top();
        stk.pop();
        cout << blk->getName() << endl;
        // set coordinates
        if(blk->getParent() == NULL){
            blk->setX(0);
            mylist.insert(0, blk->getWidth(), blk->getHeight());
        }
        else if(blk->getParent()->getLeft() == blk){
            // left child
            blk->setX(blk->getParent()->getX() + blk->getParent()->getWidth());
            mylist.insert(blk->getX(), blk->getX()+blk->getWidth(), blk->getHeight());
        }
        else{
            // right child
            blk->setX(blk->getParent()->getX());
            mylist.insert(blk->getX(), blk->getX()+blk->getWidth(), blk->getHeight());
        }

        if(blk->getRight() != NULL){
            stk.push(blk->getRight());
        }
        if(blk->getLeft() != NULL){
            stk.push(blk->getLeft());
        }
        mylist.print();
    }
    height = mylist.getMaxHeight();
    width = mylist.getMaxWidth();
    if(height > _outline_h || width > _outline_w)
        return false;
    return true;
}

void Floorplanner::genSolution(int times = 10){
    cout << "..build initial tree " << endl;
    _root = buildTree(_root, 0);
    while(times > 0){
        int width = 0, height = 0;
        while(!evaluateFeasible(width, height)){
            cout << "get infeasible "<< width << " " << height << endl;
            width = 0;
            height = 0;
            perturb();
        }
        cout << "get fasible "<< width << " " << height << endl;
        --times;
    }
}
void Floorplanner::simulateAnnealing(){
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
    printf("[Outline]: (%d, %d) \n", _outline_w, _outline_h);
    printf("Area: (%d / %d) \n", _total_sizes, _outline_w * _outline_h);
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