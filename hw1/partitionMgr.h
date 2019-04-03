#include <unordered_map>
#include "net.h"
#include "cell.h"

class PartitionMgr{
public:
    void setDegree(double d) {degree = d; }
    Cell* registerCell(string c);
    Net* registerNet(string n);
    void parse(const char* fname);
    void output();

private:
    size_t getToken(size_t pos, string& s, string& token);
    double degree;
    unordered_map <string, Net*> total_net_;
    unordered_map <string, Cell*> total_cell_;
};