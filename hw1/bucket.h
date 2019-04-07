#include "cell.h"


class LinkedList{
public:
    LinkedList(int gain);
    // void PrintList();           // 印出list的所有資料
    void push_front(Cell* c);     // 在list的開頭新增node
    void push_back(Cell* c);      // 在list的尾巴新增node
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
    void init(int size);
    Cell* getMaxGain();

private:
    vector<LinkedList*> bucket_list_;
};