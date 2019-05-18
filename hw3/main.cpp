#include <iostream>
#include <sstream>
#include <ctime>
#include "floorplanner.h"
using namespace std;

int main(int argc, char** argv)
{
    if(argc != 5){
        cout << "Wrong input format\n";
        return -1;
    }
    double d = stod(argv[1]);
    std::stringstream ss;
    time_t start, end;
    start = time(NULL);
    Floorplanner* planner = new Floorplanner(d);
    planner->parseBlocks(argv[2]);
    planner->parseNets(argv[3]);
    planner->showStatus();
    planner->getNormFactor();
    planner->simulateAnnealing(ss);
    end = time(NULL);
    planner->output(argv[4], difftime(end, start), ss);
    return 0;
}