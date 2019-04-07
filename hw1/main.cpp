#include <iostream>
#include "partitionMgr.h"
using namespace std;

int main(int argc, char** argv){
    if(argc != 3){
        return -1;
    }
    PartitionMgr mgr;
    mgr.parse(argv[1]);
    mgr.initPartition();
    mgr.output();
    return 0;
}