#include <vector>
#include <unordered_map>
#include "block.h"
#include "terminal.h"
#include "net.h"

class Floorplanner{
public:
    Floorplanner(double a) {_alpha = a;}
    void parseBlocks(const char* file);
    void parseNets(const char* file);
    // for tree struture
    Block* buildTree(Block* root, int index);
    bool evaluateFeasible(int& width, int& height);
    void deleteNode(Block* block);
    void insertNode(Block* block, Block* insert, bool isLeft);
    void swapNode(Block* block, Block* block2);
    void perturb();
    // SA
    void genSolution(int times);
    void simulateAnnealing();
    void showStatus();

private:
    double _alpha;
    int _outline_w;
    int _outline_h;
    int _total_sizes;
    Block* _root;
    vector <Net*>  _nets;
    // vector <Terminal*> _terminals;
    vector <Block* > _blocks_V;
    unordered_map<string, Block*> _blocks;
    unordered_map<string, Terminal*> _terminals;
};