#include <iostream>
using namespace std;
#include "floorplanner.h"

int main(int argc, char** argv)
{
    if(argc != 5){
        cout << "Wrong input format\n";
        return -1;
    }
    double d = stod(argv[1]);
    Floorplanner* planner = new Floorplanner(d);
    planner->parseBlocks(argv[2]);
    planner->parseNets(argv[3]);
    planner->showStatus();
    planner->getNormFactor();
    planner->simulateAnnealing();
    return 0;
}