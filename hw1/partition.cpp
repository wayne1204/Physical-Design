#include <fstream>
#include <iostream>
#include <utility>

#include "partitionMgr.h"
using namespace std;

Cell* PartitionMgr::registerCell(string c){
    if(total_cell_.find(c) == total_cell_.end()){
        Cell* cell = new Cell();
        total_cell_.emplace(make_pair(c, cell));
        return cell;
    }
    else{
        return total_cell_.find(c)->second;
    }
}

Net* PartitionMgr::registerNet(string n){
    if(total_net_.find(n) == total_net_.end()){
        Net* net = new Net();
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
        size_t pos = 0;
        getToken(pos, line, token);
        if(token != "NET")
            break;

        getToken(pos, line, token);
        Net* net = registerNet(token);

        while(token != ";"){
            getToken(pos, line, token);
        }
    }
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