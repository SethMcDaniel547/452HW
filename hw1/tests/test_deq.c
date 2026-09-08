#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "unity.h"
#include "../src/deq.h"

//deq to use
static Deq deq = NULL;
//unity functions
void setUp(void) {
    deq = NULL;
}

void tearDown(void) {
    if (deq != NULL) {
        deq_del(deq, NULL);
        deq = NULL;
    }
}

//Data conversion helper functions
int data_to_int(Data d) {
    return (int)(intptr_t)d;
}

Data int_to_data(int i) {
    return (void *)(intptr_t)i;
}

//deq setup helper functions
Deq setup_deq_empty() {
    return deq_new();
}

Deq setup_deq_1() {
    Deq q = setup_deq_empty();
    deq_head_put(q, int_to_data(1));
    deq_head_put(q, int_to_data(2));
    deq_head_put(q, int_to_data(3));
    deq_head_put(q, int_to_data(4));
    deq_head_put(q, int_to_data(5));
    return q;
}

Deq setup_deq_2() {
    Deq q = setup_deq_empty();
    deq_tail_put(q, int_to_data(1));
    deq_tail_put(q, int_to_data(2));
    deq_tail_put(q, int_to_data(3));
    deq_tail_put(q, int_to_data(4));
    deq_tail_put(q, int_to_data(5));
    return q;
}

//Test Cases
void test_new(void) {
    deq = setup_deq_empty();
    bool testExp = deq != NULL;
    TEST_ASSERT_TRUE(testExp);
}

void test_len_0(void) {
    deq = setup_deq_empty();
    bool testExp = deq_len(deq) == 0;
    TEST_ASSERT_TRUE(testExp);
}

void test_len_1(void) {
    deq = setup_deq_empty();
    deq_head_put(deq, int_to_data(5));
    bool testExp = deq_len(deq) == 1;
    TEST_ASSERT_TRUE(testExp);
}

void test_len_5(void) {
    deq = setup_deq_1();
    bool testExp = deq_len(deq) == 5;
    TEST_ASSERT_TRUE(testExp);
}

void test_head_put(void) {
    deq = setup_deq_1();
    bool testExp = data_to_int(deq_head_get(deq)) == 5 && data_to_int(deq_tail_get(deq)) == 1;
    TEST_ASSERT_TRUE(testExp);
}

void test_head_get(void) {
    deq = setup_deq_1();

    int returnA = data_to_int(deq_head_get(deq));
    int returnB = data_to_int(deq_head_get(deq));
    int returnC = data_to_int(deq_head_get(deq));
    int returnD = data_to_int(deq_head_get(deq));
    int returnE = data_to_int(deq_head_get(deq));

    //returns 0 since empty
    int returnF = data_to_int(deq_head_get(deq));

    bool testExp = returnA == 5 && returnB == 4 && returnC == 3 && returnD == 2 && returnE == 1 && returnF == 0;
    TEST_ASSERT_TRUE(testExp);
}

void test_head_ith(void) {
    deq = setup_deq_1();

    int returnA = data_to_int(deq_head_ith(deq,0));
    int returnB = data_to_int(deq_head_ith(deq,2));
    int returnC = data_to_int(deq_head_ith(deq,4));

    int returnD = data_to_int(deq_head_ith(deq,6));
    bool returnLength = deq_len(deq) == 5;

    bool testExp = returnA == 5 && returnB == 3 && returnC == 1 && returnD == 0 && returnLength;
    TEST_ASSERT_TRUE(testExp);
}

void test_head_rem(void) {
    deq = setup_deq_1();
    int returnA = data_to_int(deq_head_rem(deq,int_to_data(3)));
    bool returnLength = deq_len(deq) == 4;

    bool testExp = returnA == 3 && returnLength;
    TEST_ASSERT_TRUE(testExp);
}

void test_tail_put(void) {
    deq = setup_deq_2();
    bool testExp = data_to_int(deq_tail_get(deq)) == 5 && data_to_int(deq_head_get(deq)) == 1;
    TEST_ASSERT_TRUE(testExp);
}

void test_tail_get(void) {
    deq = setup_deq_2();

    int returnA = data_to_int(deq_tail_get(deq));
    int returnB = data_to_int(deq_tail_get(deq));
    int returnC = data_to_int(deq_tail_get(deq));
    int returnD = data_to_int(deq_tail_get(deq));
    int returnE = data_to_int(deq_tail_get(deq));

    //returns 0 since empty
    int returnF = data_to_int(deq_tail_get(deq));

    bool testExp = returnA == 5 && returnB == 4 && returnC == 3 && returnD == 2 && returnE == 1 && returnF == 0;
    TEST_ASSERT_TRUE(testExp);
}

void test_tail_ith(void) {
    deq = setup_deq_2();
    int returnA = data_to_int(deq_head_ith(deq,0));
    int returnB = data_to_int(deq_head_ith(deq,2));
    int returnC = data_to_int(deq_head_ith(deq,4));

    int returnD = data_to_int(deq_head_ith(deq,6));
    bool returnLength = deq_len(deq) == 5;

    bool testExp = returnA == 1 && returnB == 3 && returnC == 5 && returnD == 0 && returnLength;
    TEST_ASSERT_TRUE(testExp);
}

void test_tail_rem(void) {
    deq = setup_deq_2();
    int returnA = data_to_int(deq_tail_rem(deq,int_to_data(2)));
    bool returnLength = deq_len(deq) == 4;

    bool testExp = returnA == 2 && returnLength;
    TEST_ASSERT_TRUE(testExp);
}

int main(void) {
    UNITY_BEGIN();

    //test deq_new
    RUN_TEST(test_new);

    //test deq_len
    RUN_TEST(test_len_0);
    RUN_TEST(test_len_1);
    RUN_TEST(test_len_5);

    //test deq_head_put
    RUN_TEST(test_head_put);

    //test deq_head_get
    RUN_TEST(test_head_get);

    //test deq_head_ith
    RUN_TEST(test_head_ith);

    //test deq_head_rem
    RUN_TEST(test_head_rem);

    //test deq_tail_put
    RUN_TEST(test_tail_put);

    //test deq_tail_get
    RUN_TEST(test_tail_get);

    //test deq_tail_ith
    RUN_TEST(test_tail_ith);

    //test deq_tail_rem
    RUN_TEST(test_tail_rem);

    //Testing Stats
    return UNITY_END();
}