#pragma once

#include <vector>
#include <algorithm>
#include "IHeap.h"
#include "SkewHeap.h"

class LeftistHeap : public IHeap {
private:
    struct LeftistHeapElement_ {
        int value;
        int rank;
        LeftistHeapElement_ *leftChild;
        LeftistHeapElement_ *rightChild;

        explicit LeftistHeapElement_ (int value_) : value(value_), rank(1), leftChild(NULL), rightChild(NULL) {}
        ~LeftistHeapElement_ () {
            if (leftChild != NULL) {
                delete leftChild;
            }
            if (rightChild != NULL) {
                delete rightChild;
            }
        }
    };

    LeftistHeapElement_ *meld_(LeftistHeapElement_ *root1, LeftistHeapElement_ *root2);

    LeftistHeapElement_ *heap_ = NULL;

public:
    explicit LeftistHeap (int _key) { heap_ = new LeftistHeapElement_(_key); }
    explicit LeftistHeap (LeftistHeapElement_ *_ptr) { heap_ = _ptr; }
    ~LeftistHeap () { if (heap_ != NULL) { delete heap_; } }
    void insert (int _key) override;
    int getMin () override;
    void extractMin () override;
    void merge (IHeap &_root) override;
    bool empty () override;
};

LeftistHeap::LeftistHeapElement_ *LeftistHeap::meld_(LeftistHeapElement_ *root1, LeftistHeapElement_ *root2) {
    if (root1 == NULL) {
        std::swap(root1, root2);
    } else if (root2 != NULL) {
        if (root1->value > root2->value) {
            std::swap(root1, root2);
        }
        root1->rightChild = meld_(root1->rightChild, root2);
        if (root1->leftChild == NULL || root1->rightChild == NULL) {
            root1->rank = 1;
        } else {
            root1->rank = std::min(root1->leftChild->rank, root1->rightChild->rank) + 1;
        }

        if (root1->leftChild == NULL || (root1->rightChild == NULL && root1->leftChild->rank < root1->rightChild->rank)) {
            std::swap(root1->leftChild, root1->rightChild);
        }
    }
    return root1;
}


void LeftistHeap::insert (int _key) {
    LeftistHeap tmpHeap(_key);
    merge(tmpHeap);
}

int LeftistHeap::getMin () {
    return heap_->value;
}

void LeftistHeap::extractMin () {
    LeftistHeapElement_ *tmp = heap_;
    heap_ = meld_(heap_->rightChild, heap_->leftChild);
    tmp->leftChild = tmp->rightChild = NULL;
    delete tmp;
}

void LeftistHeap::merge (IHeap &_root) {
    LeftistHeap &root = dynamic_cast<LeftistHeap&>(_root);
    heap_ = meld_(heap_, root.heap_);
    root.heap_ = NULL;
}

bool LeftistHeap::empty () {
    return (heap_ == NULL);
}

