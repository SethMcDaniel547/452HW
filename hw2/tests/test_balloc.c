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
    RUN_TEST(test_bfree_and_full_coalescing);
    return UNITY_END();
}