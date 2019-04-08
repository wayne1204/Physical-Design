#include "cell.h"


class LinkedList{
public:
    LinkedList(int gain);
    // void PrintList();           // 印出list的所有資料
    void push_front(Cell* c);     // 在list的開頭新增node
    void push_back(Cell* c);      // 在list的尾巴新增node
    Cell* get_item();
    int getSize() {return size_;}
    void remove(Cell* c);         // 刪除list中的 int x
    void clear();               // 把整串list刪除
    void reverse();             // 將list反轉: 7->3->14 => 14->3->7

private:
    int size_;                // size是用來記錄Linked list的長度, 非必要
    int gain_;
    Cell *first;            // list的第一個node
};

class Bucket{
public:
    Bucket(){
        total_cells_ = 0;
    }
    int getSize() {return total_cells_;}
    void setMaxGain(int gain);
    void setBound(double up, double low);
    void insert(Cell* c);
    void remove(Cell* c);
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