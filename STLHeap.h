#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include "IHeap.h"

class STLHeap : public IHeap {
private:
    std::priority_queue< int, std::vector<int>, std::greater<int> > heap_;
public:
    STLHeap (int _key) { heap_.push(_key); }
    void insert (int key) override;
    int getMin () override;
    void extractMin () override;
    void merge (IHeap &_root) override;
    bool empty () override;
};

void STLHeap::insert (int key) {
    heap_.push(key);
}

int STLHeap::getMin () {
    return heap_.top();
}

void STLHeap::extractMin () {
    heap_.pop();
}

void STLHeap::merge (IHeap &_root) {
    STLHeap &root = dynamic_cast<STLHeap &> (_root);

    while (!root.heap_.empty()) {
        heap_.push(root.heap_.top());
        root.heap_.pop();
    }
}

bool STLHeap::empty () {
    return heap_.empty();
}

