#pragma once

#include <vector>
#include <algorithm>
#include "IHeap.h"

class LHeap : public IHeap {
private:
    struct LeftistHeapElement {
        int value;
        int rank;
        LeftistHeapElement *leftChild;
        LeftistHeapElement *rightChild;

        explicit LeftistHeapElement (int value_) : value(value_), rank(1), leftChild(NULL), rightChild(NULL) {}
        ~LeftistHeapElement () {
            if (leftChild != NULL) {
                delete leftChild;
            }
            if (rightChild != NULL) {
                delete rightChild;
            }
        }
    };

    LeftistHeapElement *meld(LeftistHeapElement *root1, LeftistHeapElement *root2);

    std::vector<LeftistHeapElement*> heaps;

public:
    ~LHeap () { for (auto it = heaps.begin(); it != heaps.end(); ++it) { delete *it; } }
    size_t addHeap (int key);
    void insert (size_t index, int key);
    int getMin (size_t index);
    void extractMin (size_t index);
    void meld (size_t index1, size_t index2);
    bool empty (size_t index);
    size_t size ();
};

LHeap::LeftistHeapElement *LHeap::meld(LeftistHeapElement *root1, LeftistHeapElement *root2) {
    if (root1 == NULL) {
        std::swap(root1, root2);
    } else if (root2 != NULL) {
        if (root1->value > root2->value) {
            std::swap(root1, root2);
        }
        root1->rightChild = meld(root1->rightChild, root2);
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


size_t LHeap::addHeap (int key) {
    heaps.push_back(new LeftistHeapElement(key));
    return static_cast<size_t>(heaps.size() - 1);
}

void LHeap::insert (size_t index, int key) {
    heaps.push_back(new LeftistHeapElement(key));
    meld(index, heaps.size() - 1);
    heaps.pop_back();
}

int LHeap::getMin (size_t index) {
    return heaps[index]->value;
}

void LHeap::extractMin (size_t index) {
    LeftistHeapElement *tmp = heaps[index];
    heaps[index] = tmp->leftChild;
    heaps.push_back(tmp->rightChild);
    tmp->leftChild = tmp->rightChild = NULL;
    delete tmp;

    meld(index, heaps.size() - 1);
    heaps.pop_back();
}

void LHeap::meld (size_t index1, size_t index2) {
    heaps[index1] = meld(heaps[index1], heaps[index2]);
    heaps[index2] = NULL;
}

bool LHeap::empty (size_t index) {
    return (heaps[index] == NULL);
}

size_t LHeap::size () {
    return heaps.size();
}
