#pragma once

#include <vector>
#include <algorithm>
#include "IHeap.h"

class SHeap : public IHeap {
private:
    struct SkewHeapElement {
        int value;
        SkewHeapElement *leftChild;
        SkewHeapElement *rightChild;

        explicit SkewHeapElement (int value_) : value(value_), leftChild(NULL), rightChild(NULL) {}
        ~SkewHeapElement () {
            if (leftChild != NULL) {
                delete leftChild;
            }
            if (rightChild != NULL) {
                delete rightChild;
            }
        }
    };

    SkewHeapElement *meld(SkewHeapElement *root1, SkewHeapElement *root2);

    std::vector<SkewHeapElement*> heaps;

public:
    ~SHeap () { for (auto it = heaps.begin(); it != heaps.end(); ++it) { delete *it; } }
    size_t addHeap (int key);
    void insert (size_t index, int key);
    int getMin (size_t index);
    void extractMin (size_t index);
    void meld (size_t index1, size_t index2);
    bool empty (size_t index);
    size_t size ();
};

SHeap::SkewHeapElement *SHeap::meld(SkewHeapElement *root1, SkewHeapElement *root2) {
    if (root1 == NULL) {
        std::swap(root1, root2);
    } else if (root2 != NULL) {
        if (root1->value > root2->value) {
            std::swap(root1, root2);
        }
        root1->rightChild = meld(root1->rightChild, root2);
        std::swap(root1->leftChild, root1->rightChild);
    }
    return root1;
}


size_t SHeap::addHeap (int key) {
    heaps.push_back(new SkewHeapElement(key));
    return static_cast<size_t>(heaps.size() - 1);
}

void SHeap::insert (size_t index, int key) {
    heaps.push_back(new SkewHeapElement(key));
    meld(index, heaps.size() - 1);
    heaps.pop_back();
}

int SHeap::getMin (size_t index) {
    return heaps[index]->value;
}

void SHeap::extractMin (size_t index) {
    SkewHeapElement *tmp = heaps[index];
    heaps[index] = tmp->leftChild;
    heaps.push_back(tmp->rightChild);
    tmp->leftChild = tmp->rightChild = NULL;
    delete tmp;

    meld(index, heaps.size() - 1);
    heaps.pop_back();
}

void SHeap::meld (size_t index1, size_t index2) {
    heaps[index1] = meld(heaps[index1], heaps[index2]);
    heaps[index2] = NULL;
}

bool SHeap::empty (size_t index) {
    return (heaps[index] == NULL);
}

size_t SHeap::size () {
    return heaps.size();
}
