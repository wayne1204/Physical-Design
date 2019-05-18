#include <iostream>
#include <fstream>
#include <stack>
#include <cassert>
#include <stdlib.h>
#include "limits.h"
#include <math.h>

#include "floorplanner.h"
#include "block.h"
using namespace std;

void Floorplanner::parseBlocks(const char* file){
    fstream fs(file, ios::in);
    string token, name;
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
            _block_num = atoi(token.c_str());
        }
        else if(token == "NumTerminals:"){
            fs >> token;
            _terminal_num = atoi(token.c_str());
            for(int i = 0; i < _block_num; ++i){
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
            for(int i = 0; i < _terminal_num; ++i){
                fs >> token;
                name = token;
                fs >> token >> token;
                int x = atoi(token.c_str());
                fs >> token;
                int y = atoi(token.c_str());
                // Terminal* ter = new Terminal(name, x, y);
                // _terminals.emplace(name, ter);
                Block* ter = new Block(name, 0, 0);
                ter->setX(x);
                ter->setY(y);
                _blocks.emplace(name, ter);
                _terminals_V.push_back(ter);
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
            n->addBlock(it->second);
        }
        _nets.push_back(n);
    }
}

Block* Floorplanner::buildTree(Block* root, int index){
    if(index < _block_num){
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
        if(block == block->getParent()->getLeft()){
            block->getParent()->setLeft(NULL);
        }else{
            block->getParent()->setRight(NULL);
        }
        block->setParent(NULL);
    }
    // case 2. one child
    else if (block->getLeft() == NULL){
        if(block == _root){
            _root = block->getRight();
            _root->setParent(NULL);
        }
        else{
            if(block == block->getParent()->getLeft()){
                block->getParent()->setLeft(block->getRight());
            }else{
                block->getParent()->setRight(block->getRight());
            }
            block->getRight()->setParent(block->getParent());
        }
        block->setParent(NULL);
        block->setRight(NULL);
    }   
    else if(block->getRight() == NULL){
        if(block == _root){
            _root = block->getLeft();
            _root->setParent(NULL);
        }
        else{
            if(block == block->getParent()->getLeft()){
                block->getParent()->setLeft(block->getLeft());
            }else{
                block->getParent()->setRight(block->getLeft());
            }
            block->getLeft()->setParent(block->getParent());
        }
        block->setParent(NULL);
        block->setLeft(NULL);
    }
    // case 3. two child
    else{
        while(block->getLeft() != NULL || block->getRight() != NULL){
            bool isLeft = block->getLeft() != NULL;
            Block* replace = (isLeft) ? block->getLeft() : block->getRight();
            if(block == _root){
                _root = replace;
                _root->setParent(NULL);
            }
            if(block == block->getParent()->getLeft()){
                block->getParent()->setLeft(replace);
            }else{
                block->getParent()->setRight(replace);
            }
            replace->setParent(block->getParent());
        }
    }
}

void Floorplanner::insertNode(Block* block, Block* insert, bool isLeft){
    insert->setLeft(NULL);
    insert->setRight(NULL);
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
    bool isLeft_1;
    if(block1->getParent() != NULL)
        isLeft_1 = (block1 == block1->getParent()->getLeft());
    Block* p = block1->getParent();
    Block* l = block1->getLeft();
    Block* r = block1->getRight();

    block1->setParent(block2->getParent());
    if(block2->getParent() == NULL){
        _root = block1;
    }
    else{
        if(block2 == block2->getParent()->getLeft()){
            block2->getParent()->setLeft(block1);
        }else{
            block2->getParent()->setRight(block1);
        }
    }
    
    block1->setLeft(block2->getLeft());
    if(block2->getLeft() != NULL){
        block2->getLeft()->setParent(block1);
    }
    block1->setRight(block2->getRight());
    if(block2->getRight() != NULL){
        block2->getRight()->setParent(block1);
    }
    
    block2->setParent(p);
    if(p == NULL){
        _root = block2;
    }
    else{
        if(isLeft_1){
            p->setLeft(block2);
        }else{
            p->setRight(block2);
        }
    }

    block2->setLeft(l);
    if(l != NULL)
        l->setParent(block2);
    block2->setRight(r);
    if(r != NULL)
        r->setParent(block2);
}

void Floorplanner::perturb(){
    _solution_space = _blocks_V;
    int action = rand() % 3;
    int block_1 = rand() % _block_num;
    int block_2 = rand() % _block_num;
    while(block_2 == block_1){
        block_2 = rand() % _block_num;
    }
    Block* b1 = _blocks_V[block_1];
    Block* b2 = _blocks_V[block_2];

    if(action == 0){
        // cout << "rotate " << _blocks_V[block_1]->getName() << "\n";
        _blocks_V[block_1]->rotate();
    }
    else if(action == 1){
        if(b1 ->getLeft() != NULL && b1->getRight() != NULL){
            // noob solution
            return;
        }
        int r = rand() % 2;
        // cout << "delete " << b1->getName() << 
        //      " & insert below " << b2->getName() <<" left " << r << "\n";
        deleteNode(b1);
        insertNode(b2, b1, r);
    }
    else{
        if(b1 == b2->getParent() || b2 == b1->getParent()){
            // noob solution
            return;
        }
        // cout << "swap " <<_blocks_V[block_1]->getName() << 
        //      " & " << _blocks_V[block_2]->getName() <<  "\n";
        swapNode(_blocks_V[block_1], _blocks_V[block_2]);
    }
}

bool Floorplanner::evaluate(int& width, int& height, int& length){
    stack<Block*> stk;
    LinkedList mylist;
    stk.push(_root);
    while(!stk.empty()){
        Block* blk = stk.top();
        stk.pop();
        // cout << blk->getName() << " " ;
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
        // mylist.print();
    }
    height = mylist.getMaxHeight();
    width = mylist.getMaxWidth();
    for(int i = 0; i < _nets.size(); ++i){
        length += _nets[i]->getWireLength();
    }
    if(height > _outline_h || width > _outline_w)
        return false;
    return true;
}

void Floorplanner::getNormFactor(){
    cout << "..build initial tree " << endl;
    _root = buildTree(_root, 0);
    int times = (_block_num + _terminal_num) * 2;
    int total_area = 0, total_l = 0;
    for(int i = 0; i < times; ++i){
        int width = 0, height = 0, length = 0;
        evaluate(width, height, length);
        // _blocks_V = bb;
        cout << "get solution " << width << " " << height << " " << length << "\n";
        total_area += (width*height);
        total_l += length;
        perturb();
    }
    _area_norm = (total_area / times);
    _wire_norm = (total_l / times);
    cout << _area_norm << " " << _wire_norm << endl;
}

void Floorplanner::simulateAnnealing(){
    double temp = 1000;
    double gamma = 0.9;
    int threshold = 1;
    int w = 0, h = 0, l = 0;
    evaluate(w, h ,l);
    double current_cost = _alpha * w * h / _area_norm + (1 - _alpha) * l / _wire_norm;
    printf("current_cost %.4f \n", current_cost);
    while (temp > threshold)
    {
        perturb();
        int w = 0, h = 0, l = 0;
        evaluate(w, h ,l);
        cout << "get solution " << w << " " << h << " " << l << "\n";
        double cost = w * h / _area_norm + l / _wire_norm;
        if(cost < current_cost){
            current_cost = cost;
            
        } 
        else{
            double prob = (double) rand() / (RAND_MAX + 1.0);
            if(prob < exp((current_cost - cost)/ temp)){
                current_cost = cost;
            }else{
                // restore
                // _blocks_V = bb;
            }
        }
        temp *= gamma;
    }
}

void Floorplanner::showStatus(){
    printf("===== [info ] =====\n");
    printf("[alpha]: %f \n",  _alpha);
    printf("[Outline]: (%d, %d) \n", _outline_w, _outline_h);
    printf("Area: (%d / %d) \n", _total_sizes, _outline_w * _outline_h);
    printf("[Blocks] nums: %d \n", _block_num);
    // for(int i = 0; i < _block_num; ++i){
    //     _blocks_V[i]->showInfo();
    // }
    printf("[Terminals] nums: %d \n", _terminal_num);
    // for(int i = 0; i < _terminal_num; ++i){
    //     _terminals_V[i]->showInfo();
    // }
    printf("[Nets] nums: %d\n", _nets.size());
    // for(int i = 0; i < _nets.size(); ++i){
    //     _nets[i]->showInfo();
    // }
}