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
        pre_ = NULL;
    }
    void setPartition(bool b) {isSetA_ = b; }
    bool getPartition() {return isSetA_;}
    void Lock() {isLocked_ = true;}
    void reset();
    bool isLocked() {return isLocked_;}
    bool incrementGain(); 
    bool decrementGain();
    int getGain() {return gain_;}
    string getName() {return name_;}
    void addNet(Net* n) {connected_nets_.push_back(n);}
    int getNetNum() {return connected_nets_.size();}
    vector<Net*>& getNets() {return connected_nets_;}

    // for linked list
    void setNext(Cell* c) {next_ = c;}
    Cell* getNext() {return next_;}
    void setPre(Cell* c) {pre_ = c; }
    Cell* getPre() {return pre_;}
private:
    bool isSetA_;
    bool isLocked_;
    int gain_;
    Cell* next_;
    Cell* pre_;
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
    void removeCell(Cell* c, bool setA);
    void reset();
    int getASize() {return partition_a_.size();}
    int getBSize() {return partition_b_.size();}
    set<Cell*>& GetPartionA() {return partition_a_;}
    set<Cell*>& GetPartionB() {return partition_b_;}

private:
    string name_;
    set<Cell*> partition_a_;
    set<Cell*> partition_b_;
};
#endif