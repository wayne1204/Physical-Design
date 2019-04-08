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
    bool done = false;

    getline(fs, line);
    setDegree(stod(line));

    while(getline(fs, line)){
        size_t pos = 0;
        pos = getToken(pos, line, token);
            
        // register netfor first time
        pos = getToken(pos, line, token);
        Net* net = registerNet(token);
        done = false;

        int count = 0;
        while(getToken(pos, line, token) != string::npos){
            pos = getToken(pos, line, token);
            if(token == ";"){
                break;
                done = true;
            }
            Cell* cell = registerCell(token);
            cell->addNet(net);
            ++count;
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
    bucket_a_->setMaxGain(max_pin_num);
    bucket_a_->setBound(up, low);
    bucket_b_->setMaxGain(max_pin_num);
    bucket_b_->setBound(up, low);

    // split cell to two sets
    for(auto it = total_cell_.begin(); it != total_cell_.end(); ++it){
        bool SetA = count < size;
        it->second->setPartition(SetA);
        if(SetA){
            bucket_a_->insert(it->second);
            init_a.insert(it->second);
        }else{
            bucket_b_->insert(it->second);
            init_b.insert(it->second);
        }
        vector<Net*> nets = it->second->getNets();
        for(int j = 0; j < nets.size(); ++j){
            nets[j]->addCell(it->second, SetA);
        } 
        ++count;
    }
    // check each net
    for(auto it = total_cell_.begin(); it != total_cell_.end(); ++it){
        vector<Net*> nets = it->second->getNets();
        bool isA = it->second->getPartition();
        Bucket* bucket = isA ? bucket_a_ : bucket_b_;
        int gain = it->second->getGain();

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
        bucket->update(it->second, gain);
    }
    printf("Finish initial partition. Criteria: %.2f - %.2f \n", low, up);
}

void PartitionMgr::updateGain(Cell* base_cell){
    vector<Net*> nets = base_cell->getNets();
    bool isA = base_cell->getPartition();

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
    // for(int i = total_cell_.size() - 1 ; i > index; --i){
        
    //     if(moving[i]->getPartition()){
    //         bucket_a_->remove(moving[i]);
    //         bucket_b_->insert(moving[i]);
    //     }else{
    //         bucket_b_->remove(moving[i]);
    //         bucket_a_->insert(moving[i]);
    //     }
    // }
    // for(int i = 0; i < total_cell_.size(); ++i){
    //     total_cell_[i].
    // }
}

bool PartitionMgr::moveCell(){
    int partial_sum = 0, index = 0, max = 0;
    
    for(int i = 0; i < total_cell_.size(); ++i){
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
            exit(0);
            break;
        }
        
        Cell* cell = isA ? a : b;
        cout << "#" << i+1 << ": moving " << cell->getName() 
        << " with gain " << cell->getGain();
        if(isA)
            cout << " from A to B \r";
        else
            cout << " from B to A \r";
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
    cout << "Largest Partial Sum " << max << " | # of moving cells:" << index <<endl;
    cout << "A size:" << bucket_a_->getSize() << " B size:" << bucket_b_->getSize() <<endl;
    return (partial_sum > 0);
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

void PartitionMgr::output(){
    cout <<"output file" << endl;

    // for(unordered_map <string, Cell*>::iterator it = total_cell_.begin(); it != total_cell_.end(); ++it){
    //     cout << it->first << " " << it->second->getGain() << endl;
    // }
}