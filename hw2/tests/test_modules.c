#include "unity.h"
#include "../src/utils.h"
#include "../src/bm.h"
#include "../src/bbm.h"
#include "../src/freelist.h"

static void *base;
static FreeList list;

void setUp(void) {
    base = NULL;
    list = NULL;
}

void tearDown(void) {
    if (list) {
        freelistdelete(list, 4, 6);
        list = NULL;
    }
    if (base) {
        mmfree(base, 64);
        base = NULL;
    }
}

void test_utils_size_helpers(void) {
    TEST_ASSERT_EQUAL_INT(4, divup(10, 3));
    TEST_ASSERT_EQUAL_INT(2, bits2bytes(9));
    TEST_ASSERT_EQUAL_INT(32, e2size(5));
    TEST_ASSERT_EQUAL_INT(0, size2e(1));
    TEST_ASSERT_EQUAL_INT(4, size2e(16));
    TEST_ASSERT_EQUAL_INT(5, size2e(17));
}

void test_utils_bit_operations(void) {
    unsigned char bytes[2] = {0, 0};

    bitset(bytes, 0);
    bitset(bytes, 9);
    TEST_ASSERT_EQUAL_INT(1, bittst(bytes, 0));
    TEST_ASSERT_EQUAL_INT(1, bittst(bytes, 9));
    TEST_ASSERT_EQUAL_INT(0, bittst(bytes, 1));

    bitinv(bytes, 0);
    TEST_ASSERT_EQUAL_INT(0, bittst(bytes, 0));
    bitclr(bytes, 9);
    TEST_ASSERT_EQUAL_INT(0, bittst(bytes, 9));
}

void test_bm_set_clear_and_test(void) {
    BM bitmap = bmcreate(10);

    TEST_ASSERT_NOT_NULL(bitmap);
    for (size_t i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT(0, bmtst(bitmap, i));
    }
    bmset(bitmap, 3);
    bmset(bitmap, 9);
    TEST_ASSERT_EQUAL_INT(1, bmtst(bitmap, 3));
    TEST_ASSERT_EQUAL_INT(1, bmtst(bitmap, 9));
    bmclr(bitmap, 3);
    TEST_ASSERT_EQUAL_INT(0, bmtst(bitmap, 3));
    bmdelete(bitmap);
}

void test_bbm_address_helpers(void) {
    unsigned char memory[64] = {0};
    void *first = memory;
    void *second = memory + 16;

    TEST_ASSERT_EQUAL_PTR(second, baddrset(memory, first, 4));
    TEST_ASSERT_EQUAL_PTR(first, baddrclr(memory, second, 4));
    TEST_ASSERT_EQUAL_PTR(first, baddrinv(memory, second, 4));
    TEST_ASSERT_NOT_EQUAL(0, baddrtst(memory, second, 4));
}

void test_bbm_tracks_buddy_pairs(void) {
    unsigned char memory[64] = {0};
    BBM bitmap = bbmcreate(64, 4);

    TEST_ASSERT_NOT_NULL(bitmap);
    TEST_ASSERT_EQUAL_INT(0, bbmtst(bitmap, memory, memory, 4));
    bbmset(bitmap, memory, memory + 16, 4);
    TEST_ASSERT_EQUAL_INT(1, bbmtst(bitmap, memory, memory, 4));
    TEST_ASSERT_EQUAL_INT(1, bbmtst(bitmap, memory, memory + 16, 4));
    bbmclr(bitmap, memory, memory, 4);
    TEST_ASSERT_EQUAL_INT(0, bbmtst(bitmap, memory, memory + 16, 4));
    bbmdelete(bitmap);
}

void test_freelist_allocates_requested_levels(void) {
    void *small;
    void *large;

    base = mmalloc(64);
    TEST_ASSERT_NOT_NULL(base);
    list = freelistcreate(64, base, 4, 6);
    TEST_ASSERT_NOT_NULL(list);

    small = freelistalloc(list, base, 4, 4);
    large = freelistalloc(list, base, 5, 4);
    TEST_ASSERT_NOT_NULL(small);
    TEST_ASSERT_NOT_NULL(large);
    TEST_ASSERT_EQUAL_INT(4, freelistsize(list, base, small, 4, 6));
    TEST_ASSERT_EQUAL_INT(5, freelistsize(list, base, large, 4, 6));
}

void test_freelist_free_coalesces_buddies(void) {
    void *first;
    void *second;
    void *whole;

    base = mmalloc(64);
    TEST_ASSERT_NOT_NULL(base);
    list = freelistcreate(64, base, 4, 6);
    TEST_ASSERT_NOT_NULL(list);

    first = freelistalloc(list, base, 4, 4);
    second = freelistalloc(list, base, 4, 4);
    TEST_ASSERT_NOT_NULL(first);
    TEST_ASSERT_NOT_NULL(second);

    freelistfree(list, base, first, 4, 4);
    freelistfree(list, base, second, 4, 4);
    whole = freelistalloc(list, base, 6, 4);
    TEST_ASSERT_EQUAL_PTR(base, whole);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_utils_size_helpers);
    RUN_TEST(test_utils_bit_operations);
    RUN_TEST(test_bm_set_clear_and_test);
    RUN_TEST(test_bbm_address_helpers);
    RUN_TEST(test_bbm_tracks_buddy_pairs);
    RUN_TEST(test_freelist_allocates_requested_levels);
    RUN_TEST(test_freelist_free_coalesces_buddies);
    return UNITY_END();
}
