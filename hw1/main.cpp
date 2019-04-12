#include <iostream>
#include "partitionMgr.h"
using namespace std;

int main(int argc, char** argv){
    if(argc != 3){
        return -1;
    }
    PartitionMgr mgr;
    int count = 0;

    mgr.parse(argv[1]);
    mgr.initPartition();
    
    cout << "\nIteration# 0 \n";
    mgr.initGain();
    while(mgr.moveCell()){
        cout << "\nIteration#" << ++count <<endl;
        mgr.initGain();
    }
    mgr.output(argv[2]);
    return 0;
}