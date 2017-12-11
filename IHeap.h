#pragma once

#include <algorithm>

class IHeap {
public:
    virtual size_t addHeap (int key) = 0;
    virtual void insert (size_t index, int key) = 0;
    virtual int getMin (size_t index) = 0;
    virtual void extractMin (size_t index) = 0;
    virtual void meld (size_t index1, size_t index2) = 0;
    virtual bool empty (size_t index) = 0;
    virtual size_t size () = 0;
};