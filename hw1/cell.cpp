#include "cell.h"
#include <set>
void Net::addCell(Cell* c, bool setA){
    if(setA){
        partition_a_.insert(c);
    }
    else{
        partition_b_.insert(c);
    }
}