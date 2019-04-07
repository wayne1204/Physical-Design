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
        bucket_a_size = 0;
        bucket_b_size = 0;
    }
    void setDegree(double d) {degree = d; }
    Cell* registerCell(string c);
    Net* registerNet(string n);

    void parse(const char* fname);
    void initPartition();
    void updateGain();
    void output();

private:
    size_t getToken(size_t pos, string& s, string& token);
    int max_pin_num;
    int bucket_a_size;
    int bucket_b_size;
    double degree;
    Bucket* bucket_a_;
    Bucket* bucket_b_;
    // unordered_set<string> set_a_;
    // unordered_set<string> set_b_;
    unordered_map <string, Net*> total_net_;
    unordered_map <string, Cell*> total_cell_;
};

#endif