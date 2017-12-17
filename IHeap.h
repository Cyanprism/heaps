#pragma once

#include <algorithm>

class IHeap {
public:
    //virtual ~IHeap () = 0;
    virtual void insert (int _key) = 0;
    virtual int getMin () = 0;
    virtual void extractMin () = 0;
    virtual void merge (IHeap &_root) = 0;
    virtual bool empty () = 0;
};