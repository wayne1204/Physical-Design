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
    int count;
    while(mgr.moveCell()){
        cout << "Iteration#" << count++ <<endl;
    }
    mgr.output();
    return 0;
}