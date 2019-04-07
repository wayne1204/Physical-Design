#ifndef CELL_H
#define CELL_H

#include <vector>
#include <set>
#include <string>
using namespace std;

class Net;

class Cell{
public:
    Cell(){
        name_= "undefined";
    }
    Cell(string n){
        name_ = n;
        isLocked_ = false;
        gain_ = 0;
        next_ = NULL;
    }
    void setPartition(bool b) {isSetA_ = b; }
    bool getPartition() {return isSetA_;}
    void Lock() {isLocked_ = true;}
    void incrementGain() {if(!isLocked_)++gain_;}
    void decrementGain() {if(!isLocked_)--gain_; }
    int getGain() {return gain_;}
    string getName() {return name_;}
    void addNet(Net* n) {connected_nets_.push_back(n);}
    vector<Net*> getNets() {return connected_nets_;}

    void setNext(Cell* c) {next_ = c;}
    Cell* getNext() {return next_;}
private:
    bool isSetA_;
    bool isLocked_;
    int gain_;
    Cell* next_;
    string name_;
    std::vector<Net*> connected_nets_;
};

class Net{
public:
    Net(){
        name_ = "undefined";
    }
    Net(string n){
        name_ = n;
    }
    void addCell(Cell* c, bool setA);
    int getASize() {return partition_a_.size();}
    int getBSize() {return partition_b_.size();}


private:
    string name_;
    set<Cell*> partition_a_;
    set<Cell*> partition_b_;

    std::vector<Cell*> connected_cells_;
};
#endif