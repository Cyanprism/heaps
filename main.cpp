#include <iostream>
#include "gtest/gtest.h"
#include "IHeap.h"
#include "BHeap.h"
#include "LHeap.h"
#include "SHeap.h"
#include "STDHeap.h"

void testHeaps (IHeap &heap1, IHeap &heap2, int iterations = 5000000);

TEST (BHeap, Test) {
    BHeap heap1;
    STDHeap heap2;
    ASSERT_NO_THROW(testHeaps(heap1, heap2));
}

TEST (LHeap, Test) {
    LHeap heap1;
    STDHeap heap2;
    ASSERT_NO_THROW(testHeaps(heap1, heap2));
}

TEST (SHeap, Test) {
    SHeap heap1;
    STDHeap heap2;
    ASSERT_NO_THROW(testHeaps(heap1, heap2));
}

void testHeaps (IHeap &heap1, IHeap &heap2, int iterations) {
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