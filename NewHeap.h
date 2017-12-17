#pragma once

#include <vector>
#include <algorithm>
#include "IHeap.h"

template <class TElement>
class NewHeap;

struct Base_ {
    int value;
    Base_ *leftChild;
    Base_*rightChild;

    explicit Base_ (int _value) : value(_value), leftChild(NULL), rightChild(NULL) {}
    virtual ~Base_ () {
        if (leftChild != NULL) {
            delete leftChild;
        }
        if (rightChild != NULL) {
            delete rightChild;
        }
    }

    virtual void additional () = 0;
};

struct SkewHeap : public Base_ {
    explicit SkewHeap (int _value) : Base_(_value) {}
    ~SkewHeap () = default;
    void additional () {
        std::swap(this->leftChild, this->rightChild);
    }
};

struct LeftistHeap : public Base_ {
    int rank;
    explicit LeftistHeap (int _value) : Base_(_value), rank(1) {}
    ~LeftistHeap () = default;
    void additional () {
        if (this->leftChild == NULL || this->rightChild == NULL) {
            this->rank = 1;
        } else {
            this->rank = std::min(dynamic_cast<LeftistHeap*>(this->leftChild)->rank,
                                  dynamic_cast<LeftistHeap*>(this->rightChild)->rank) + 1;
        }

        if (this->leftChild == NULL || (this->rightChild == NULL &&
                dynamic_cast<LeftistHeap*>(this->leftChild)->rank < dynamic_cast<LeftistHeap*>(this->rightChild)->rank))
        {
            std::swap(this->leftChild, this->rightChild);
        }
    }
};

template <class TElement>
class NewHeap : public IHeap {
private:
    TElement *meld_(TElement *root1, TElement *root2);

    TElement *heap_ = NULL;

public:
    explicit NewHeap (int _key) { heap_ = new TElement(_key); }
    explicit NewHeap (TElement *_ptr) { heap_ = _ptr; }
    ~NewHeap () { if (heap_ != NULL) { delete heap_; } }
    void insert (int _key) override;
    int getMin () override;
    void extractMin () override;
    void merge (IHeap &_root) override;
    bool empty () override;
};

template<class TElement>
TElement *NewHeap<TElement>::meld_(TElement *root1, TElement *root2) {
    if (root1 == NULL) {
        std::swap(root1, root2);
    } else if (root2 != NULL) {
        if (root1->value > root2->value) {
            std::swap(root1, root2);
        }
        root1->rightChild = meld_(dynamic_cast<TElement*>(root1->rightChild), root2);
        root1->additional();
    }
    return root1;
}

template <class TElement>
void NewHeap<TElement>::insert (int _key) {
    NewHeap tmpHeap(_key);
    merge(tmpHeap);
}

template <class TElement>
int NewHeap<TElement>::getMin () {
    return heap_->value;
}

template <class TElement>
void NewHeap<TElement>::extractMin () {
    TElement *tmp = heap_;
    heap_ = meld_(dynamic_cast<TElement*>(heap_->rightChild),
                  dynamic_cast<TElement*>(heap_->leftChild));
    tmp->leftChild = tmp->rightChild = NULL;
    delete tmp;
}

template <class TElement>
void NewHeap<TElement>::merge (IHeap &_root) {
    NewHeap &root = dynamic_cast<NewHeap&>(_root);
    heap_ = meld_(heap_, root.heap_);
    root.heap_ = NULL;
}

template <class TElement>
bool NewHeap<TElement>::empty () {
    return (heap_ == NULL);
}

