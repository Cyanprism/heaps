#include <iostream>
#include "gtest/gtest.h"
#include "IHeap.h"
#include "BinomialHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include "STLHeap.h"

void testTwoHeaps (HeapInterface &heap1, HeapInterface &heap2, int iterations = 5000000);

TEST (BHeap, Test) {
    HeapInterface<BinomialHeap> heap1;
    HeapInterface<STLHeap> heap2;
    ASSERT_NO_THROW(testTwoHeaps(heap1, heap2));
}

TEST (LeftistHeap, Test) {
    HeapInterface<LeftistHeap> heap1;
    HeapInterface<STLHeap> heap2;
    ASSERT_NO_THROW(testTwoHeaps(heap1, heap2));
}

TEST (SkewHeap, Test) {
    HeapInterface<SkewHeap> heap1;
    HeapInterface<STLHeap> heap2;
    ASSERT_NO_THROW(testTwoHeaps(heap1, heap2));
}

template <class THeap>
class HeapInterface {
private:
    std::vector<THeap> array_;
public:
    explicit HeapInterface () = default;
    void addHeap (int _key);
    void insert (int _index, int _key);
    int getMin (int _index);
    void extractMin (int _index);
    void meld (int _index1, int _index2);
    bool empty (int _index);
    int size ();
};

template <class THeap>
void HeapInterface<THeap>::addHeap (int _key) {
    array_.push_back(THeap(_key));
}

template <class THeap>
void HeapInterface<THeap>::insert (int _index, int _key) {
    array_[_index].insert(_key);
}

template <class THeap>
int HeapInterface<THeap>::getMin (int _index) {
    return array_[_index].getMin();
}

template <class THeap>
void HeapInterface<THeap>::extractMin (int _index) {
    array_[_index].extractMin();
}

template <class THeap>
void HeapInterface<THeap>::meld (int _index1, int _index2) {
    array_[_index1].merge(array_[_index2]);
}

template <class THeap>
bool HeapInterface<THeap>::empty (int _index) {
    return array_[_index].empty();
}

template <class THeap>
int HeapInterface<THeap>::size () {
    return array_.size();
}

void testTwoHeaps (HeapInterface &heap1, HeapInterface &heap2, int iterations = 5000000) {
    for (int i = 0; i < iterations; ++i) {
        if (heap1.size() != heap2.size()) {
            std::cout << i << std::endl;
            throw;
        }

        int type = rand() % 5;

        if (type == 0) {
            int key = rand();

            heap1.addHeap(key);
            heap2.addHeap(key);
        } else if (type == 1 && heap1.size() > 0) {
            int index = rand() % heap1.size();
            int key = rand();

            heap1.insert(index, key);
            heap2.insert(index, key);
        } else if (type == 2 && heap1.size() > 0) {
            int index = rand() % heap1.size();

            if (heap1.empty(index) != heap2.empty(index) ||
                (!heap1.empty(index) && heap1.getMin(index) != heap2.getMin(index)))
            {
                std::cout << i << std::endl;
                throw;
            }
        } else if (type == 3 && heap1.size() > 0) {
            int index = rand() % heap1.size();

            if (!heap1.empty(index)) {
                heap1.extractMin(index);
                heap2.extractMin(index);
            }
        } else if (type == 4 && heap1.size() > 1) {
            int index1 = rand() % heap1.size();
            int index2 = rand() % heap1.size();

            if (index1 == index2) {
                index2 = (index1 + 1) % heap1.size();
            }

            heap1.meld(index1, index2);
            heap2.meld(index1, index2);
        }
    }
}
