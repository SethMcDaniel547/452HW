#include "unity.h"
#include "../src/balloc.h"

static Balloc ba = NULL;

void setUp(void) {
    // Create a pool of 4096 bytes (2^12), min=16 (2^4), max request=1024 (2^10)
    ba = bcreate(4096, 4, 10);
}

void tearDown(void) {
    if (ba) {
        bdelete(ba);
        ba = NULL;
    }
}

void test_bcreate_valid(void) {
    TEST_ASSERT_NOT_NULL(ba);
}

void test_balloc_rounding_and_sizing_correct_size(void) {
    bdelete(ba);
    ba = bcreate(1024, 4, 10);
    void *p1 = balloc(ba, 10);
    TEST_ASSERT_NOT_NULL(p1);
    //bprint(ba);
    TEST_ASSERT_EQUAL_INT(16, bsize(ba, p1));
}

void test_balloc_rounding_and_sizing(void) {
    // 10 bytes should round up to min block size 2^4 = 16 bytes
    void *p1 = balloc(ba, 10);
    TEST_ASSERT_NOT_NULL(p1);
    //bprint(ba);
    TEST_ASSERT_EQUAL_INT(16, bsize(ba, p1));

    // 200 bytes should round up to 2^8 = 256 bytes
    void *p2 = balloc(ba, 200);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_EQUAL_INT(256, bsize(ba, p2));

    // 800 bytes should round up to 2^10 = 1024 bytes
    void *p3 = balloc(ba, 800);
    TEST_ASSERT_NOT_NULL(p3);
    TEST_ASSERT_EQUAL_INT(1024, bsize(ba, p3));
}

void test_balloc_constraints(void) {
    // Requesting larger than u (2000 bytes > 2^10 = 1024) should return NULL
    void *p_fail = balloc(ba, 2000);
    TEST_ASSERT_NULL(p_fail);
}

void test_balloc_upper_boundary(void) {
    void *p_max = balloc(ba, 1024);
    void *p_over = balloc(ba, 1025);

    TEST_ASSERT_NOT_NULL(p_max);
    TEST_ASSERT_EQUAL_INT(1024, bsize(ba, p_max));
    TEST_ASSERT_NULL(p_over);
    bfree(ba, p_max);
}

void test_balloc_exhaustion_and_reuse(void) {
    Balloc small = bcreate(64, 4, 4);
    void *blocks[4];

    TEST_ASSERT_NOT_NULL(small);
    for (int i = 0; i < 4; i++) {
        blocks[i] = balloc(small, 16);
        TEST_ASSERT_NOT_NULL(blocks[i]);
    }
    TEST_ASSERT_NULL(balloc(small, 16));

    bfree(small, blocks[1]);
    TEST_ASSERT_NOT_NULL(balloc(small, 16));
    bdelete(small);
}

void test_bcreate_non_power_of_two_size(void) {
    Balloc non_power = bcreate(3000, 4, 10);
    void *p1;
    void *p2;

    TEST_ASSERT_NOT_NULL(non_power);
    p1 = balloc(non_power, 1024);
    p2 = balloc(non_power, 1024);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_EQUAL_INT(1024, bsize(non_power, p1));
    TEST_ASSERT_EQUAL_INT(1024, bsize(non_power, p2));
    bdelete(non_power);
}

void test_bsize_after_free(void) {
    void *p = balloc(ba, 100);

    TEST_ASSERT_NOT_NULL(p);
    bfree(ba, p);
    TEST_ASSERT_EQUAL_INT(0, bsize(ba, p));
}

void test_repeated_create_delete(void) {
    for (int i = 0; i < 100; i++) {
        Balloc temporary = bcreate(4096, 4, 10);
        TEST_ASSERT_NOT_NULL(temporary);
        bdelete(temporary);
    }
}

void test_double_free_does_not_duplicate_block(void) {
    Balloc small = bcreate(64, 4, 4);
    void *first = balloc(small, 16);
    void *second;
    void *third;
    void *fourth;
    void *fifth;

    TEST_ASSERT_NOT_NULL(small);
    TEST_ASSERT_NOT_NULL(first);
    bfree(small, first);
    bfree(small, first);

    second = balloc(small, 16);
    third = balloc(small, 16);
    fourth = balloc(small, 16);
    fifth = balloc(small, 16);
    TEST_ASSERT_NOT_NULL(second);
    TEST_ASSERT_NOT_NULL(third);
    TEST_ASSERT_NOT_NULL(fourth);
    TEST_ASSERT_NOT_NULL(fifth);
    TEST_ASSERT_NULL(balloc(small, 16));
    bdelete(small);
}

void test_bfree_and_full_coalescing(void) {
    void *p1 = balloc(ba, 10);   // 16 bytes
    void *p2 = balloc(ba, 200);  // 256 bytes
    void *p3 = balloc(ba, 800);  // 1024 bytes

    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_NOT_NULL(p3);

    // Free in sequence and verify no crashes
    bfree(ba, p2);
    bfree(ba, p1);
    bfree(ba, p3);

    // After freeing everything, allocating the full max pool size 
    // should succeed, proving complete root coalescing happened.
    void *p_full = balloc(ba, 1024);
    //bprint(ba);
    TEST_ASSERT_NOT_NULL(p_full);
    TEST_ASSERT_EQUAL_INT(1024, bsize(ba, p_full));
    bfree(ba, p_full);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_bcreate_valid);
    RUN_TEST(test_balloc_rounding_and_sizing_correct_size);
    RUN_TEST(test_balloc_rounding_and_sizing);
    RUN_TEST(test_balloc_constraints);
    RUN_TEST(test_balloc_upper_boundary);
    RUN_TEST(test_balloc_exhaustion_and_reuse);
    RUN_TEST(test_bcreate_non_power_of_two_size);
    RUN_TEST(test_bsize_after_free);
    RUN_TEST(test_repeated_create_delete);
    RUN_TEST(test_double_free_does_not_duplicate_block);
    RUN_TEST(test_bfree_and_full_coalescing);
    return UNITY_END();
}