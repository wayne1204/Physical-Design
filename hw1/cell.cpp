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