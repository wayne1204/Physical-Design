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
    void deleteNode(Block* block);
    void insertNode(Block* block, Block* insert, bool isLeft);
    void swapNode(Block* block, Block* block2);
    int perturb();
    bool evaluate(int& width, int& height, int& length);
    // SA
    void getNormFactor();
    void simulateAnnealing(stringstream& ss);
    void store(stringstream& ss);
    void output(char* fname, double t, stringstream& ss);
    void showStatus();

private:
    int _outline_w;
    int _outline_h;
    int _total_sizes;
    int _block_num;
    int _terminal_num;
    int _area_norm;
    int _wire_norm;
    double _alpha;
    Block* _root;
    Block* _original_root;
    vector <Net*>  _nets;
    vector <Block* > _terminals_V;
    vector <Block* > _blocks_V;
    vector <Block* > _original;
    unordered_map<string, Block*> _blocks;
};