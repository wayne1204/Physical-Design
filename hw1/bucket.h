#include "cell.h"


class LinkedList{
public:
    LinkedList(int gain);
    void push_front(Cell* c);    
    Cell* get_item();
    int getSize() {return size_;}
    void remove(Cell* c);        
    void clear();               

private:
    int size_;
    int gain_;
    Cell *first;
};

class Bucket{
public:
    Bucket(){
        total_cells_ = 0;
    }
    int getSize() {return total_cells_;}
    void init(int max_gain, double low, double up);
    // void setMaxGain(int gain);
    // void setBound(double up, double low);
    void insert(Cell* c);
    void remove(Cell* c);
    void clear();
    void update(Cell* c, int old_gain);
    Cell* getMaxGain();

private:
    vector<LinkedList*> bucket_list_;
    int total_cells_;
    int max_gain_;
    int max_gain_ptr_;
    double low_bound;
    double up_bound;
};