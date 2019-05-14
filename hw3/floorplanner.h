#include <vector>
#include <unordered_map>
#include "block.h"
#include "terminal.h"
#include "net.h"

class Floorplanner{
public:
    Floorplanner(double a) {_alpha = a;}
    void setCoordinates(int w, int h) {_outline_w = w;  _outline_h = h;}
    void parseBlocks(const char* file);
    void parseNets(const char* file);
    Block* buildTree(Block* root, int index);
    void genSolution();
    void simulateAnnealing();
    void showStatus();

private:
    double _alpha;
    int _outline_w;
    int _outline_h;
    Block* _root;
    vector <Net*>  _nets;
    // vector <Terminal*> _terminals;
    vector <Block* > _blocks_V;
    unordered_map<string, Block*> _blocks;
    unordered_map<string, Terminal*> _terminals;
};