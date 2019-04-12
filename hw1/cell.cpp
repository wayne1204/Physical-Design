#include "cell.h"
#include <set>

bool Cell::incrementGain(){
    if(isLocked_)
        return false;
    ++gain_;
    return true;
}

bool Cell::decrementGain(){
    if(isLocked_)
        return false;
    --gain_;
    return true;
}

void Cell::reset(){
    isLocked_ = false;
    gain_ = 0;
    next_ = NULL;
    pre_ = NULL;
}

void Net::addCell(Cell* c, bool setA){
    if(setA){
        partition_a_.insert(c);
    }
    else{
        partition_b_.insert(c);
    }
}

void Net::removeCell(Cell* c, bool setA){
    if(setA){
        partition_a_.erase(c);
    }else
    {
        partition_b_.erase(c);
    }
    
}

void Net::reset(){
    partition_a_.clear();
    partition_b_.clear();
}