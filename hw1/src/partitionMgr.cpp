#include <fstream>
#include <iostream>
#include <math.h>
#include <utility>
#include <cassert>

#include "partitionMgr.h"
using namespace std;

Cell* PartitionMgr::registerCell(string c){
    assert(c != ";");
    if(total_cell_.find(c) == total_cell_.end()){
        Cell* cell = new Cell(c);
        total_cell_.emplace(make_pair(c, cell));
        return cell;
    }
    else{
        return total_cell_.find(c)->second;
    }
}

Net* PartitionMgr::registerNet(string n){
    if(total_net_.find(n) == total_net_.end()){
        Net* net = new Net(n);
        total_net_.emplace(make_pair(n, net));
        return net;
    }
    else{
        return total_net_.find(n)->second;
    }
}

void PartitionMgr::parse(const char* fname){
    ifstream fs(fname);
    string line, token;

    getline(fs, line);
    setDegree(stod(line));
    Net* net;

    while(getline(fs, line)){
        // cout << line <<endl;
        size_t pos = 0;
        pos = getToken(pos, line, token);

        // register netfor first time
        if(token == "NET"){
            pos = getToken(pos, line, token);
            net = registerNet(token);
        }
        else if(token != ";"){
            // cout << token;
            Cell* cell = registerCell(token);
            cell->addNet(net);
        }

        while(getToken(pos, line, token) != string::npos){
            pos = getToken(pos, line, token);
            // cout << token;
            if(token == ";"){
                break;
            }
            Cell* cell = registerCell(token);
            cell->addNet(net);
        }
        if(token != ";" && token != ""){
            // cout << token << token.size() << endl;
            Cell* cell = registerCell(token);
            cell->addNet(net);
        }
    }
    for(auto it = total_cell_.begin(); it != total_cell_.end(); ++it){
        max_pin_num = max(max_pin_num, it->second->getNetNum());
    }
    cout << "Finish parsing with " << total_cell_.size() <<" cells and "
    << total_net_.size() << " nets. maxGain:" << max_pin_num << "\n";
}

// first half to cell A, 
void PartitionMgr::initPartition(){
    int size = ceil(total_cell_.size()/2);
    int count = 0;
    double low = (1 - degree) * total_cell_.size() / 2;
    double up = (1 + degree) * total_cell_.size() / 2;
    bucket_a_->init(max_pin_num, low, up);
    bucket_b_->init(max_pin_num, low, up);

    // split cell to two sets
    for(auto it = total_cell_.begin(); it != total_cell_.end(); ++it){
        bool SetA = count < size;
        it->second->setPartition(SetA);
        if(SetA){
            bucket_a_->insert(it->second);
            set_a_initial.insert(it->second);
        }else{
            bucket_b_->insert(it->second);
            set_b_initial.insert(it->second);
        }
        vector<Net*> nets = it->second->getNets();
        for(int j = 0; j < nets.size(); ++j){
            nets[j]->addCell(it->second, SetA);
        } 
        ++count;
    }
    printf("Finish initial partition. Criteria: %.2f - %.2f \n", low, up);
}

void PartitionMgr::initGain(){
    // check each net
    for(auto it = total_cell_.begin(); it != total_cell_.end(); ++it){
        bool isA = it->second->getPartition();
        Bucket* bucket = isA ? bucket_a_ : bucket_b_;
        vector<Net*> nets = it->second->getNets();
        assert(it->second->getGain() == 0);
        for(int j = 0; j < nets.size(); ++j){
            int fromSize = isA ? nets[j]->getASize() : nets[j]->getBSize();
            int toSize   = isA ? nets[j]->getBSize() : nets[j]->getASize();
           
            if(fromSize == 1){
                it->second->incrementGain();
            }
            if(toSize == 0){
                it->second->decrementGain();
            }
        } 
        bucket->update(it->second, 0);
    }
    printf("Finish init gain, cut size = %d \n", countCost());
}

void PartitionMgr::updateGain(Cell* base_cell){
    vector<Net*> nets = base_cell->getNets();
    bool isA = base_cell->getPartition();
    base_cell->setPartition(!isA);
    for(int j = 0; j < nets.size(); ++j){
        int fromSize = isA ? nets[j]->getASize() : nets[j]->getBSize();
        int toSize   = isA ? nets[j]->getBSize() : nets[j]->getASize();
        set<Cell*> fromSet = isA ? nets[j]->GetPartionA() : nets[j]->GetPartionB();
        set<Cell*> toSet =   isA ? nets[j]->GetPartionB() : nets[j]->GetPartionA();
        Bucket* bucket = isA ? bucket_a_ : bucket_b_;
        // before move
        if(toSize == 0){
            for(auto it = fromSet.begin(); it != fromSet.end(); ++it){
                if((*it)->incrementGain()){
                    if((*it)->getPartition())
                        bucket_a_->update((*it), (*it)->getGain()-1);
                    else
                        bucket_b_->update((*it), (*it)->getGain()-1);
                }
            }
        }
        if(toSize == 1){
            for(auto it = toSet.begin(); it != toSet.end(); ++it){
                if((*it)->decrementGain()){
                    if((*it)->getPartition())
                        bucket_a_->update((*it), (*it)->getGain()+1);
                    else
                        bucket_b_->update((*it), (*it)->getGain()+1);
                }
                   
            }
        }
        // move
        nets[j]->removeCell(base_cell, isA);
        nets[j]->addCell(base_cell, !isA);
        fromSize--;
        toSize++;
        // after move
        if(fromSize == 0){
            for(auto it = toSet.begin(); it != toSet.end(); ++it){
                if((*it)->decrementGain()){
                    if((*it)->getPartition())
                        bucket_a_->update((*it), (*it)->getGain()+1);
                    else
                        bucket_b_->update((*it), (*it)->getGain()+1);
                }
            }
        }
        if(fromSize == 1){
            for(auto it = fromSet.begin(); it != fromSet.end(); ++it){
                if((*it)->incrementGain()){
                    if((*it)->getPartition())
                        bucket_a_->update((*it), (*it)->getGain()-1);
                    else
                        bucket_b_->update((*it), (*it)->getGain()-1);
                }
            }
        }
    } 
}

void PartitionMgr::reconstruct(int index){
    // reset cell & net
    for(auto it = total_cell_.begin(); it != total_cell_.end(); ++it){
        it->second->reset();
    }
    for(auto it = total_net_.begin(); it != total_net_.end(); ++it){
        it->second->reset();
    }

    // reset bucket
    bucket_a_->clear();
    bucket_b_->clear();
    moving.clear();
    for(auto it = set_a_initial.begin(); it != set_a_initial.end(); ++it){
        (*it)->setPartition(true);
        bucket_a_->insert(*it);
        vector<Net*> nets = (*it)->getNets();
        for(int j = 0; j < nets.size(); ++j){
            nets[j]->addCell(*it, true);
        } 
    }
    for(auto it = set_b_initial.begin(); it != set_b_initial.end(); ++it){
        (*it)->setPartition(false);
        bucket_b_->insert(*it);
        vector<Net*> nets = (*it)->getNets();
        for(int j = 0; j < nets.size(); ++j){
            nets[j]->addCell(*it, false);
        } 
    }
    
    // perform movement
    for(int i = 0; i <= index; ++i){
        bool isA = moving[i]->getPartition();
        if(isA){
            bucket_a_->remove(moving[i]);
            bucket_b_->insert(moving[i]);
            set_a_initial.erase(moving[i]);
            set_b_initial.insert(moving[i]);
        }else{
            bucket_b_->remove(moving[i]);
            bucket_a_->insert(moving[i]);
            set_b_initial.erase(moving[i]);
            set_a_initial.insert(moving[i]);
        }

        moving[i]->setPartition(!isA);
        vector<Net*> nets = moving[i]->getNets();
        for(int j = 0; j < nets.size(); ++j){
            nets[j]->removeCell(moving[i], isA);
            nets[j]->addCell(moving[i], !isA);
        }
    }
    cout << "A size:" << bucket_a_->getSize() 
    << " B size:" << bucket_b_->getSize() <<endl;
}

bool PartitionMgr::moveCell(int iter){
    int partial_sum = 0, index = 0, max = 0;
    int times = total_cell_.size();
    if(iter <= 2)
        times = ceil(times * 0.7);
    else if(iter <= 5)
        times = ceil(times *  0.5);
    else if(iter <= 10)
        times = ceil(times * 0.3);
    else
        times = ceil(times * 0.2);

    for(int i = 0; i < times; ++i){
        bool isA; 
        Cell* a = bucket_a_->getMaxGain();
        Cell* b = bucket_b_->getMaxGain();
        
        if(a != NULL && b != NULL)
            isA = a->getGain() > b->getGain() ? true : false;
        else if(a != NULL)
            isA = true;
        else if(b != NULL)
            isA = false;
        else{
            cout << "break\n";
            break;
        }
        
        Cell* cell = isA ? a : b;
        // cout << "#" << i+1 << ": moving " << cell->getName() 
        // << " with gain " << cell->getGain();
        // if(isA)
        //     cout << " from A to B \r";
        // else
        //     cout << " from B to A \r";
        partial_sum += cell->getGain();
        
        if(partial_sum > max){
            max = partial_sum;
            index = i;
        }
        moving.push_back(cell);
        cell->Lock();
        updateGain(cell);
        if(isA){
            bucket_a_->remove(cell);
            bucket_b_->insert(cell);
            
        }else{
            bucket_b_->remove(cell);
            bucket_a_->insert(cell);
        }
    }
    printf("Largest Gain:%d (at step: %d) | ", max , index+1);
    reconstruct(index);
    return (max > 0);
}

int PartitionMgr::countCost(){
    int cost = 0;
    for(auto it = total_net_.begin(); it != total_net_.end(); ++it){
        int a = it->second->getASize();
        int b = it->second->getBSize();
        if(a * b){
            ++cost;
        }
    }
    return cost;
}

size_t PartitionMgr::getToken(size_t pos, string& s, string& token){
    size_t begin = s.find_first_not_of(' ', pos);
    if(begin == string::npos){
        token = "";
        return begin;
    }
    size_t end = s.find_first_of(' ', begin);
    token = s.substr(begin, end - begin);
    return end; 
}

void PartitionMgr::output(const char* fname){
    ofstream fs(fname);
    fs << "Cutsize = " << countCost() <<endl;
    fs << "G1 " << set_a_initial.size() << endl;
    for(auto it = set_a_initial.begin(); it != set_a_initial.end(); ++it){
        fs << (*it)->getName() << ' ';
    }
    fs << ";\n";
    fs << "G2 " << set_b_initial.size() << endl;
    for(auto it = set_b_initial.begin(); it != set_b_initial.end(); ++it){
        fs << (*it)->getName() << ' ';
    }
    fs << ";\n";
    fs.close();
}