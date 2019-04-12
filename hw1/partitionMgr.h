#ifndef MGR
#define MGR

#include <unordered_map>
#include <unordered_set>
#include "cell.h"
#include "bucket.h"
using namespace std;

class PartitionMgr{
public:
    PartitionMgr(){
        max_pin_num = 0;
        bucket_a_ = new Bucket();
        bucket_b_ = new Bucket();
    }
    void setDegree(double d) {degree = d; }
    Cell* registerCell(string c);
    Net* registerNet(string n);

    void parse(const char* fname);
    void initPartition();
    void initGain();
    void updateGain(Cell* base_cell);
    void reconstruct(int index);
    bool moveCell();
    void output();

private:
    size_t getToken(size_t pos, string& s, string& token);
    int max_pin_num;
    double degree;
    Bucket* bucket_a_;
    Bucket* bucket_b_;

    // for reconstruct
    unordered_set <Cell*> init_a;
    unordered_set <Cell*> init_b;
    vector<Cell*> moving;

    // total
    unordered_map <string, Net*> total_net_;
    unordered_map <string, Cell*> total_cell_;
};

#endif