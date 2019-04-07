#include <fstream>
#include <iostream>
#include <math.h>
#include <utility>

#include "partitionMgr.h"
using namespace std;

Cell* PartitionMgr::registerCell(string c){
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
    cout << line <<endl;

    while(getline(fs, line)){
        cout << line <<endl;
        size_t pos = 0;
        pos = getToken(pos, line, token);
        cout << token << endl;
        if(token != "NET")
            break;

        // register for first time
        pos = getToken(pos, line, token);
        Net* net = registerNet(token);

        int count = 0;
        while(token != ";"){
            // cout << token <<endl;
            pos = getToken(pos, line, token);
            Cell* cell = registerCell(token);
            cell->addNet(net);
            // net->addCell(cell);
            ++count;
        }
        max_pin_num = max(max_pin_num, count);
    }
}

// first half to cell A, 
void PartitionMgr::initPartition(){
    int size = ceil(total_cell_.size()/2);
    bucket_a_size = size;
    bucket_b_size = total_cell_.size() - size;
    int count = 0;

    // split cell to two sets
    for(unordered_map <string, Cell*>::iterator it = total_cell_.begin(); it != total_cell_.end(); ++it){
        bool SetA = count < size;
        it->second->setPartition(SetA);
        vector<Net*> nets = it->second->getNets();
        for(int j = 0; j < nets.size(); ++j){
            nets[j]->addCell(it->second, SetA);
        } 
        ++count;
    }
    // check each net
    for(unordered_map<string, Cell*>::iterator it = total_cell_.begin(); it != total_cell_.end(); ++it){
        vector<Net*> nets = it->second->getNets();
        bool isA = it->second->getPartition();

        for(int j = 0; j < nets.size(); ++j){
            int fromSize = isA ? nets[j]->getASize() : nets[j]->getBSize();
            int toSize   = isA ? nets[j]->getBSize() : nets[j]->getASize();
           
            if(fromSize == 1)
                it->second->incrementGain();
            if(toSize == 1)
                it->second->decrementGain();
        } 
    }
}

void PartitionMgr::updateGain(){
    
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
    cout <<"# cells "<< total_cell_.size() <<endl;
    cout <<"# nets " << total_net_.size() <<endl;
    cout <<"max pin# " << max_pin_num << endl;

    // for(unordered_map <string, Cell*>::iterator it = total_cell_.begin(); it != total_cell_.end(); ++it){
    //     cout << it->first << " " << it->second->getGain() << endl;
    // }
}