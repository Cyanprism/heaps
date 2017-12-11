#pragma once

#include <vector>
#include <algorithm>
#include "IHeap.h"

class BHeap : public IHeap {
private:
    struct BinomialHeapElement {
        int value;
        std::vector<BinomialHeapElement*> next;

        explicit BinomialHeapElement (int value_) : value(value_) {}
        ~BinomialHeapElement () {
            for (auto it = next.begin(); it != next.end(); ++it) {
                delete *it;
            }
        }
    };

    BinomialHeapElement *add (BinomialHeapElement *root1, BinomialHeapElement *root2);

    struct BinomialHeap {
        std::vector<BinomialHeapElement*> elements;

        BinomialHeap () {}
        explicit BinomialHeap (int elem_) : elements(1, new BinomialHeapElement(elem_)) {}
        ~BinomialHeap () {
            for (auto it = elements.begin(); it != elements.end(); ++it) {
                if (*it != NULL) {
                    delete *it;
                }
            }
        }
    };

    std::vector<BinomialHeap*> heaps;

public:
    ~BHeap () { for (auto it = heaps.begin(); it != heaps.end(); ++it) { delete *it; }}
    size_t addHeap (int key);
    void insert (size_t index, int key);
    int getMin (size_t index);
    void extractMin (size_t index);
    void meld (size_t index1, size_t index2);
    bool empty (size_t index);
    size_t size ();
};

BHeap::BinomialHeapElement *BHeap::add (BinomialHeapElement *root1, BinomialHeapElement *root2) {
    if (root1 == NULL) {
        std::swap(root1, root2);
    }

    if (root2 != NULL) {
        if (root1->value > root2->value) {
            std::swap(root1, root2);
        }
        root1->next.push_back(root2);
    }

    return root1;
}

size_t BHeap::addHeap (int key) {
    heaps.push_back(new BinomialHeap(key));
    return static_cast<size_t>(heaps.size() - 1);
}

void BHeap::insert (size_t index, int key) {
    BinomialHeapElement *tmp = new BinomialHeapElement(key);
    for (size_t i = 0; i < heaps[index]->elements.size() && tmp != NULL; ++i) {
        if (heaps[index]->elements[i] == NULL) {
            std::swap(heaps[index]->elements[i], tmp);
        } else {
            tmp = add(tmp, heaps[index]->elements[i]);
            heaps[index]->elements[i] = NULL;
        }
    }

    if (tmp != NULL) {
        heaps[index]->elements.push_back(tmp);
    }
}

int BHeap::getMin (size_t index) {
    int result = heaps[index]->elements.back()->value;
    for (size_t i = 0; i < heaps[index]->elements.size(); ++i) {
        if (heaps[index]->elements[i] != NULL && heaps[index]->elements[i]->value < result) {
            result = heaps[index]->elements[i]->value;
        }
    }
    return result;
}

void BHeap::extractMin (size_t index) {
    int result = heaps[index]->elements.back()->value;
    size_t result_index = heaps[index]->elements.size() - 1;

    for (size_t i = 0; i < heaps[index]->elements.size(); ++i) {
        if (heaps[index]->elements[i] != NULL && heaps[index]->elements[i]->value < result) {
            result = heaps[index]->elements[i]->value;
            result_index = i;
        }
    }

    heaps.resize(heaps.size() + 1, new BinomialHeap);
    heaps.back()->elements.swap(heaps[index]->elements[result_index]->next);
    delete heaps[index]->elements[result_index];
    heaps[index]->elements[result_index] = NULL;
    while (!heaps[index]->elements.empty() && heaps[index]->elements.back() == NULL) {
        heaps[index]->elements.pop_back();
    }

    meld(index, heaps.size() - 1);
    delete heaps.back();
    heaps.pop_back();

}

void BHeap::meld (size_t index1, size_t index2) {
    BinomialHeapElement *tmp = NULL;
    heaps[index1]->elements.resize(std::max(heaps[index1]->elements.size(), heaps[index2]->elements.size()));
    size_t i = 0;
    for (; i < heaps[index2]->elements.size(); ++i) {
        if (tmp != NULL && heaps[index2]->elements[i] != NULL) {
            tmp = add(tmp, heaps[index2]->elements[i]);
            heaps[index2]->elements[i] = NULL;
        } else {
            if (heaps[index2]->elements[i] != NULL) {
                tmp = add(tmp, heaps[index2]->elements[i]);
                heaps[index2]->elements[i] = NULL;
            }
            if (heaps[index1]->elements[i] == NULL) {
                std::swap(tmp, heaps[index1]->elements[i]);
            } else if (tmp != NULL) {
                tmp = add(tmp, heaps[index1]->elements[i]);
                heaps[index1]->elements[i] = NULL;
            }
        }
    }

    for (; tmp != NULL && i < heaps[index1]->elements.size(); ++i) {
        if (heaps[index1]->elements[i] == NULL) {
            std::swap(tmp, heaps[index1]->elements[i]);
        } else if (tmp == NULL) {
            tmp = add(tmp, heaps[index1]->elements[i]);
            heaps[index1]->elements[i] = NULL;
        }
    }

    if (tmp != NULL) {
        heaps[index1]->elements.push_back(tmp);
    }
    heaps[index2]->elements.clear();
}

bool BHeap::empty (size_t index) {
    return heaps[index]->elements.empty();
}

size_t BHeap::size () {
    return heaps.size();
}
