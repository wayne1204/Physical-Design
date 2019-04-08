#include <iostream>
#include <cassert>
#include "bucket.h"
using namespace std;

LinkedList::LinkedList(int gain){
    size_ = 0;
    gain_ = gain;
    first = NULL;
}

void LinkedList::push_front(Cell* c){
    if(first != NULL){
        c->setNext(first);
        first->setPre(c);
    }
    first = c;
    ++size_;
}

void LinkedList::remove(Cell* c){
    // check head
    if(c->getName() == first->getName()){
        first = first->getNext();
        if(first != NULL)
            first->setPre(NULL);
    }else{
        c->getPre()->setNext(c->getNext());  
    }
    // check tail
    if(c->getNext() != NULL){
        c->getNext()->setPre(c->getPre());
    }
    c->setNext(NULL);
    c->setPre(NULL);
    --size_;
}

Cell* LinkedList::get_item(){
    Cell* current = first;
    if(first == NULL)
        return first;
    while(current->isLocked()){
        current = current->getNext();
        if(current == NULL) {
            break;
        }
    }
    return current;
} 

// input: max gain of a cell
void Bucket::setMaxGain(int max_gain){
    max_gain_ = max_gain;
    max_gain_ptr_ = max_gain * 2; 

    for(int i = 0; i < max_gain; ++i){
        LinkedList* list = new LinkedList(max_gain-i);
        bucket_list_.push_back(list);
    }
    LinkedList* list = new LinkedList(0);
    bucket_list_.push_back(list);
    for(int i = 1; i <= max_gain; ++i){
        LinkedList* list = new LinkedList(i*(-1));
        bucket_list_.push_back(list);
    }
}

void Bucket::setBound(double up, double low){
    assert(low < up);
    low_bound = low;
    up_bound = up;
}

void Bucket::insert(Cell* c){
    int gain = c->getGain();
    bucket_list_[max_gain_-gain]->push_front(c);
    max_gain_ptr_ = min(max_gain_ptr_, max_gain_-gain);
    total_cells_++;
}

void Bucket::remove(Cell* c){
    int gain = c->getGain();
    bucket_list_[max_gain_-gain]->remove(c);
    while(bucket_list_[max_gain_ptr_]->getSize() == 0 && max_gain_ptr_ < 2*max_gain_){
        max_gain_ptr_++;
    }
    total_cells_--;
}

void Bucket::update(Cell* c, int old_gain){
    int gain = c->getGain();
    assert(gain <= max_gain_ && gain >= max_gain_ * (-1) );
    if(gain == old_gain)
        return;
    bucket_list_[max_gain_-old_gain]->remove(c);
    bucket_list_[max_gain_-gain]->push_front(c);
    max_gain_ptr_ = min(max_gain_ptr_, max_gain_-gain);
}

Cell* Bucket::getMaxGain(){
    if(total_cells_ - 1 < low_bound){
        return NULL;
    }
    while(bucket_list_[max_gain_ptr_]->get_item() == NULL && max_gain_ptr_ < 2*max_gain_){
        max_gain_ptr_++;
    }
    return bucket_list_[max_gain_ptr_]->get_item();
}