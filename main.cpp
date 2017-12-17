#include <iostream>
#include "gtest/gtest.h"
#include "IHeap.h"
#include "BinomialHeap.h"
#include "NewHeap.h"
//#include "LeftistHeap.h"
//#include "SkewHeap.h"
#include "STLHeap.h"

template <class THeap>
class HeapInterface {
private:
    std::vector<THeap *> array_;
public:
    explicit HeapInterface () = default;
    void addHeap (int _key);
    void insert (int _index, int _key);
    int getMin (int _index);
    void extractMin (int _index);
    void meld (int _index1, int _index2);
    bool empty (int _index);
    int size ();
    void clear ();
};

template <class THeap>
void HeapInterface<THeap>::addHeap (int _key) {
    array_.push_back(new THeap(_key));
}

template <class THeap>
void HeapInterface<THeap>::insert (int _index, int _key) {
    array_[_index]->insert(_key);
}

template <class THeap>
int HeapInterface<THeap>::getMin (int _index) {
    return array_[_index]->getMin();
}

template <class THeap>
void HeapInterface<THeap>::extractMin (int _index) {
    array_[_index]->extractMin();
}

template <class THeap>
void HeapInterface<THeap>::meld (int _index1, int _index2) {
    array_[_index1]->merge(*array_[_index2]);
}

template <class THeap>
bool HeapInterface<THeap>::empty (int _index) {
    return array_[_index]->empty();
}

template <class THeap>
int HeapInterface<THeap>::size () {
    return array_.size();
}

template <class THeap>
void HeapInterface<THeap>::clear () {
    for (auto it = array_.begin(); it != array_.end(); ++it) {
        delete *it;
    }
    array_.clear();
}

class _TESTING_PARAMETRES_ {
public:
    int NUM_OF_ITERATIONS;
    int CHANCE_OF_ADD_HEAP;
    int CHANCE_OF_INSERT;
    int CHANCE_OF_GET_MIN;
    int CHANCE_OF_EXTRACT_MIN;
    int CHANCE_OF_MELD;

    _TESTING_PARAMETRES_ (int iterations = 1000000, int c1 = 1, int c2 = 1, int c3 = 1, int c4 = 1, int c5 = 1) {
        NUM_OF_ITERATIONS = iterations;
        CHANCE_OF_ADD_HEAP = c1;
        CHANCE_OF_INSERT = c2;
        CHANCE_OF_GET_MIN = c3;
        CHANCE_OF_EXTRACT_MIN = c4;
        CHANCE_OF_MELD = c5;
    }
};


enum Action { add_heap, insert, get_min, extract_min, meld };

Action getNextAction (const _TESTING_PARAMETRES_ &TEST) {
    int sum = TEST.CHANCE_OF_ADD_HEAP + TEST.CHANCE_OF_INSERT + TEST.CHANCE_OF_GET_MIN +
              TEST.CHANCE_OF_EXTRACT_MIN + TEST.CHANCE_OF_MELD;

    int action = rand() % sum;
    if (action < TEST.CHANCE_OF_ADD_HEAP) {
        return add_heap;
    } else if (action < TEST.CHANCE_OF_ADD_HEAP + TEST.CHANCE_OF_INSERT) {
        return insert;
    } else if (action < TEST.CHANCE_OF_ADD_HEAP + TEST.CHANCE_OF_INSERT + TEST.CHANCE_OF_GET_MIN) {
        return get_min;
    } else if (action < TEST.CHANCE_OF_ADD_HEAP + TEST.CHANCE_OF_INSERT + TEST.CHANCE_OF_GET_MIN +
                        TEST.CHANCE_OF_EXTRACT_MIN)
    {
        return extract_min;
    } else {
        return meld;
    }
}

int getKey () {
    return rand();
}

/*int getIndex () {
    return rand();
}*/

template <class THeap1, class THeap2 = STLHeap>
void testTwoHeaps (const _TESTING_PARAMETRES_ &TEST) {
    HeapInterface<THeap1> heap1;
    HeapInterface<THeap2> heap2;

    _TESTING_PARAMETRES_ k(5);

    for (int i = 0; i < TEST.NUM_OF_ITERATIONS; ++i) {
        //std::cout << i << std::endl;
        if (heap1.size() != heap2.size()) {
            std::cout << i << std::endl;
            throw;
        }

        Action type = getNextAction(TEST);

        if (type == add_heap) {
            int key = getKey();

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
        } else if (3 <= type && type >= 4 && heap1.size() > 0) {
            int index = rand() % heap1.size();

            if (!heap1.empty(index)) {
                heap1.extractMin(index);
                heap2.extractMin(index);
            }
        } else if (type == 5 && heap1.size() > 1) {
            int index1 = rand() % heap1.size();
            int index2 = rand() % heap1.size();

            if (index1 == index2) {
                index2 = (index1 + 1) % heap1.size();
            }

            heap1.meld(index1, index2);
            heap2.meld(index1, index2);
        }
    }

    heap1.clear(); heap2.clear();
}

TEST (BHeap, Test) {
    ASSERT_NO_THROW(testTwoHeaps<BinomialHeap>(_TESTING_PARAMETRES_()));
}

TEST (LeftistHeap, Test) {
    ASSERT_NO_THROW(testTwoHeaps<LeftistHeap>(_TESTING_PARAMETRES_()));
}

TEST (SkewHeap, Test) {
    ASSERT_NO_THROW(testTwoHeaps<SkewHeap>(_TESTING_PARAMETRES_()));
}

/*int main() {
    while (true) {
        testTwoHeaps<NewHeap<SkewHeap>>();
    }
}*/

