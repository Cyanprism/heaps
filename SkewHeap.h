#pragma once

#include <vector>
#include <algorithm>
#include "IHeap.h"

class SkewHeap : public IHeap {
private:
    struct SkewHeapElement_ {
        int value;
        SkewHeapElement_ *leftChild;
        SkewHeapElement_ *rightChild;

        explicit SkewHeapElement_ (int value_) : value(value_), leftChild(NULL), rightChild(NULL) {}
        ~SkewHeapElement_ () {
            if (leftChild != NULL) {
                delete leftChild;
            }
            if (rightChild != NULL) {
                delete rightChild;
            }
        }
    };

    SkewHeapElement_ *meld_(SkewHeapElement_ *root1, SkewHeapElement_ *root2);

    SkewHeapElement_ *heap_;

public:
    SkewHeap (int _key) { heap_ = new SkewHeapElement_(_key); }
    ~SkewHeap () { for (auto it = heaps.begin(); it != heaps.end(); ++it) { delete *it; } }
    void insert (int _key) override;
    int getMin () override;
    void extractMin () override;
    void merge (IHeap &_root) override;
    bool empty () override;
};

SkewHeap::SkewHeapElement_ *SkewHeap::meld_(SkewHeapElement_ *root1, SkewHeapElement_ *root2) {
    if (root1 == NULL) {
        std::swap(root1, root2);
    } else if (root2 != NULL) {
        if (root1->value > root2->value) {
            std::swap(root1, root2);
        }
        root1->rightChild = meld_(root1->rightChild, root2);
        std::swap(root1->leftChild, root1->rightChild);
    }
    return root1;
}


void SkewHeap::insert (int _key) {
    SkewHeap tmpHeap(_key);
    merge(tmpHeap);
}

int SkewHeap::getMin () {
    return heap_->value;
}

void SkewHeap::extractMin () {
    SkewHeapElement_ *tmp = heap_;
    heap_ = meld_(heap_->leftChild, heap_->rightChild);
    tmp->leftChild = tmp->rightChild = NULL;
    delete tmp;
}

void SkewHeap::merge (IHeap &_root) {
    SkewHeap root = dynamic_cast<SkewHeap &>(_root);
    heap_ = meld_(heap_, root.heap_);
    root.heap_ = NULL;
}

bool SkewHeap::empty () {
    return (heap_ == NULL);
}

