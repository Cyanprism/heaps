#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include "IHeap.h"

class STDHeap : public IHeap {
private:
    std::vector< std::priority_queue< int, std::vector<int>, std::greater<int> > > heaps;
public:
    size_t addHeap (int key);
    void insert (size_t index, int key);
    int getMin (size_t index);
    void extractMin (size_t index);
    void meld (size_t index1, size_t index2);
    bool empty (size_t index);
    size_t size ();
};

size_t STDHeap::addHeap (int key) {
    heaps.push_back(std::priority_queue< int, std::vector<int>, std::greater<int> >(1, key));
    return heaps.size() - 1;
}

void STDHeap::insert (size_t index, int key) {
    heaps[index].push(key);
}

int STDHeap::getMin (size_t index) {
    return heaps[index].top();
}

void STDHeap::extractMin (size_t index) {
    heaps[index].pop();
}

void STDHeap::meld (size_t index1, size_t index2) {
    while (!heaps[index2].empty()) {
        heaps[index1].push(heaps[index2].top());
        heaps[index2].pop();
    }
}

bool STDHeap::empty (size_t index) {
    return heaps[index].empty();
}

size_t STDHeap::size () {
    return heaps.size();
}
