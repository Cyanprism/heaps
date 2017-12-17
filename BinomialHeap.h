#pragma once

#include <vector>
#include <algorithm>
#include "IHeap.h"

class BinomialHeap : public IHeap {
private:
    struct BinomialHeapElement_ {
        int value;
        std::vector<BinomialHeapElement_*> next;

        explicit BinomialHeapElement_ (int _value) : value(_value) {}
        ~BinomialHeapElement_ () {
            for (auto it = next.begin(); it != next.end(); ++it) {
                delete *it;
            }
        }
    };

    BinomialHeapElement_ *mergeEqualHeapElements_ (BinomialHeapElement_ *root1, BinomialHeapElement_ *root2);

    std::vector<BinomialHeapElement_*> elements_;

public:
    BinomialHeap () = default;
    explicit BinomialHeap (int _elem) : elements_(1, new BinomialHeapElement_(_elem)) {}
    ~BinomialHeap () {
        for (auto it = elements_.begin(); it != elements_.end(); ++it) {
            if (*it != NULL) {
                delete *it;
            }
        }
    }

    void insert (int _key) override;
    int getMin () override;
    void extractMin () override;
    void merge (IHeap &_root) override;
    bool empty () override;
};

BinomialHeap::BinomialHeapElement_ *BinomialHeap::mergeEqualHeapElements_ (BinomialHeapElement_ *_root1,
                                                                          BinomialHeapElement_ *_root2)
{
    if (_root1 == NULL) {
        std::swap(_root1, _root2);
    }

    if (_root2 != NULL) {
        if (_root1->value > _root2->value) {
            std::swap(_root1, _root2);
        }
        _root1->next.push_back(_root2);
    }

    return _root1;
}

void BinomialHeap::insert (int _key) {
    BinomialHeap tmpHeap = BinomialHeap(_key);
    merge(tmpHeap);
}

int BinomialHeap::getMin () {
    int result = elements_.back()->value;
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (elements_[i] != NULL && elements_[i]->value < result) {
            result = elements_[i]->value;
        }
    }
    return result;
}

void BinomialHeap::extractMin () {
    int result = elements_.back()->value;
    size_t result_index = elements_.size() - 1;

    for (size_t i = 0; i < elements_.size(); ++i) {
        if (elements_[i] != NULL && elements_[i]->value < result) {
            result = elements_[i]->value;
            result_index = i;
        }
    }

    BinomialHeap *tmp = new BinomialHeap;
    tmp->elements_.swap(elements_[result_index]->next);
    delete elements_[result_index];
    elements_[result_index] = NULL;
    while (!elements_.empty() && elements_.back() == NULL) {
        elements_.pop_back();
    }

    this->merge(*tmp);
    delete tmp;
}

void BinomialHeap::merge (IHeap &_root) {
    BinomialHeap &root = dynamic_cast<BinomialHeap&>(_root);
    BinomialHeapElement_ *tmp = NULL;
    elements_.resize(std::max(elements_.size(), root.elements_.size()));
    size_t i = 0;
    for (; i < root.elements_.size(); ++i) {
        if (tmp != NULL && root.elements_[i] != NULL) {
            tmp = mergeEqualHeapElements_(tmp, root.elements_[i]);
            root.elements_[i] = NULL;
        } else {
            if (root.elements_[i] != NULL) {
                std::swap(tmp, root.elements_[i]);
            }
            if (elements_[i] == NULL) {
                std::swap(tmp, elements_[i]);
            } else if (tmp != NULL) {
                tmp = mergeEqualHeapElements_(tmp, elements_[i]);
                elements_[i] = NULL;
            }
        }
    }

    for (; tmp != NULL && i < elements_.size(); ++i) {
        if (elements_[i] == NULL) {
            std::swap(tmp, elements_[i]);
        } else  {
            tmp = mergeEqualHeapElements_(tmp, elements_[i]);
            elements_[i] = NULL;
        }
    }

    if (tmp != NULL) {
        elements_.push_back(tmp);
    }
    root.elements_.clear();
}

bool BinomialHeap::empty () {
    return elements_.empty();
}
