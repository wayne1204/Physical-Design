#include <iostream>
#include "bucket.h"
using namespace std;

LinkedList::LinkedList(int gain){
    size_ = 0;
    gain_ = gain;
    first = 0;
}

void LinkedList::push_front(Cell* c){
    c->setNext(first);
    first = c;
    ++size_;
}

void LinkedList::remove(Cell* c){
    Cell* current = first;
    Cell* previous = NULL;
    while(current->getName() != c->getName()){
        previous = current;
        current = current->getNext();
    }
    
    if(current->getName() == first->getName()){
        first = first->getNext();
        --size_;
    }
    else if(current == NULL){
        cout << "cell " << c->getName() << "not found!\n" <<endl;
    }
    else{
        previous->setNext(current->getNext()) ;
        --size_;
    }
}

// input: max gain of a cell
void Bucket::init(int size){
    for(int i = 0; i < size; ++i){
        LinkedList* list = new LinkedList(size-i);
        bucket_list_.push_back(list);
    }
    LinkedList* list = new LinkedList(0);
    bucket_list_.push_back(list);
    for(int i = 1; i <= size; ++i){
        LinkedList* list = new LinkedList(i*(-1));
        bucket_list_.push_back(list);
    }
}