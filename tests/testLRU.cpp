#include <gtest/gtest.h>
#include "lru/LRUCache.hpp"
#include <string>

TEST(LRU, BasicPutGetAndRecency) {
    LRUCache<int, std::string> c(2);
    c.put(1, "one");
    c.put(2, "two");

    auto v1 = c.get(1);
    ASSERT_TRUE(v1.has_value());
    EXPECT_EQ(*v1, "one");      // 1 becomes MRU

    c.put(3, "three");          // evicts LRU (key 2)

    EXPECT_FALSE(c.get(2).has_value());
    EXPECT_EQ(c.get(3).value(), "three");
    EXPECT_EQ(c.get(1).value(), "one");
}

TEST(LRU, UpdateMovesToMRU) {
    LRUCache<int, std::string> c(2);
    c.put(1, "one");
    c.put(2, "two");
    c.put(1, "uno");            // update + move to MRU
    c.put(3, "three");          // should evict key 2

    EXPECT_FALSE(c.get(2).has_value());
    EXPECT_EQ(c.get(1).value(), "uno");
}

TEST(LRU, OperatorBracketInsertsDefaultAndReturnsRef) {
    LRUCache<int, int> c(2);
    int& r = c[42];             // default insert
    r = 7;
    EXPECT_EQ(c.get(42).value(), 7);
}

TEST(LRU, CapacityZeroBehavesAsEmpty_Policy) {
    LRUCache<int, int> c(0);
    c.put(1, 10);
    EXPECT_FALSE(c.get(1).has_value());
}

TEST(LRU, EvictOnCapacityEdge) {
    LRUCache<int, int> c(1);
    c.put(1, 10);
    c.put(2, 20);               // evict 1
    EXPECT_FALSE(c.get(1).has_value());
    EXPECT_EQ(c.get(2).value(), 20);
}
TEST(LRU, ShrinkCapacityEvictsLRU) {
    LRUCache<int, int> c(3);
    c.put(1, 1); c.put(2, 2); c.put(3, 3);
    // order MRU: 3,2,1 (assuming head=MRU)
    c.setCapacity(2);
    EXPECT_FALSE(c.get(1).has_value()); // oldest evicted
    EXPECT_TRUE(c.get(2).has_value());
    EXPECT_TRUE(c.get(3).has_value());
}

TEST(LRU, GetMovesToMRU) {
    LRUCache<int, int> c(2);
    c.put(1, 10); c.put(2, 20);
    (void)c.get(1);         // 1 becomes MRU
    c.put(3, 30);            // evicts key 2
    EXPECT_FALSE(c.get(2).has_value());
    EXPECT_TRUE(c.get(1).has_value());
    EXPECT_TRUE(c.get(3).has_value());
}

