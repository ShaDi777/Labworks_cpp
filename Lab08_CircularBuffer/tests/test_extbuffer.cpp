#include <gtest/gtest.h>

#include "lib/CircularBufferExt.h"


TEST(ExtendedCircularBufferTest, EmptyTest1) {
    CircularBufferExt<std::string> buffer;

    ASSERT_TRUE(buffer.empty());
}

TEST(ExtendedCircularBufferTest, EmptyTest2) {
    CircularBufferExt<std::string> buffer(10);

    ASSERT_TRUE(buffer.empty());
}

TEST(ExtendedCircularBufferTest, EqualityTest1) {
    CircularBufferExt<std::string> buffer1 = {"1", "2", "3", "4"};
    CircularBufferExt<std::string> buffer2 = {"1", "2", "3", "4"};

    ASSERT_TRUE(buffer1 == buffer2);
}

TEST(ExtendedCircularBufferTest, EqualityTest2) {
    CircularBufferExt<std::string> buffer1 = {"1", "2", "3", "4"};
    CircularBufferExt<std::string> buffer2 = {"1", "2", "3", "4", "5"};

    ASSERT_FALSE(buffer1 == buffer2);
}

TEST(ExtendedCircularBufferTest, ConstructorsTest1) {
    std::vector<int> arr = {1, 2, 3, 4, 5};

    CircularBufferExt<int> buffer_iter(arr.begin(), arr.end());
    CircularBufferExt<int> buffer_initlist = {1, 2, 3, 4, 5};

    ASSERT_TRUE(buffer_initlist == buffer_iter);
}

TEST(ExtendedCircularBufferTest, ConstructorsTest2) {
    unsigned n = 5;
    int value = 1;
    CircularBufferExt<int> buffer_ncopies(n, value);

    CircularBufferExt<int> buffer_copy(buffer_ncopies);

    ASSERT_TRUE(buffer_copy == buffer_ncopies);
}

TEST(ExtendedCircularBufferTest, ConstructorsTest3) {
    std::vector<int> arr = {1, 1, 1, 1, 1};
    CircularBufferExt<int> buffer_iter(arr.begin(), arr.end());

    CircularBufferExt<int> buffer_initlist = {1, 1, 1, 1, 1};

    unsigned n = 5;
    int value = 1;
    CircularBufferExt<int> buffer_ncopies(n, value);

    CircularBufferExt<int> buffer_copy(buffer_iter);

    ASSERT_EQ(buffer_iter, buffer_initlist);
    ASSERT_EQ(buffer_iter, buffer_ncopies);
    ASSERT_EQ(buffer_iter, buffer_copy);
    ASSERT_EQ(buffer_initlist, buffer_ncopies);
    ASSERT_EQ(buffer_initlist, buffer_copy);
    ASSERT_EQ(buffer_ncopies, buffer_copy);
}

TEST(ExtendedCircularBufferTest, OperatorAssginTest1) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    CircularBufferExt<int> buffer_test = {5, 4, 3, 2, 1};
    buffer = {5, 4, 3, 2, 1};

    ASSERT_TRUE(buffer == buffer_test);
}

TEST(ExtendedCircularBufferTest, OperatorAssginTest2) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    CircularBufferExt<int> buffer_test = {5, 4, 3, 2, 1};
    buffer = buffer_test;

    ASSERT_TRUE(buffer == buffer_test);
}

TEST(ExtendedCircularBufferTest, OperatorAssginTest3) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    CircularBufferExt<int> buffer_test = {5, 4, 3, 2, 1};
    buffer = CircularBufferExt<int>({5, 4, 3, 2, 1});

    ASSERT_TRUE(buffer == buffer_test);
}

TEST(ExtendedCircularBufferTest, PushTest1) {
    CircularBufferExt<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    CircularBufferExt<int> buffer_test = {1, 2, 3, 4, 5};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, PushTest2) {
    CircularBufferExt<int> buffer = {3, 2, 1};
    buffer.push_front(4);
    buffer.push_front(5);

    CircularBufferExt<int> buffer_test = {5, 4, 3, 2, 1};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, PushTest3) {
    CircularBufferExt<int> buffer = {1, 2, 3};
    buffer.reserve(10);
    buffer.push_back(4);
    buffer.push_back(5);

    CircularBufferExt<int> buffer_test = {1, 2, 3, 4, 5};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, PushTest4) {
    CircularBufferExt<int> buffer = {3, 2, 1};
    buffer.reserve(10);
    buffer.push_front(4);
    buffer.push_front(5);

    CircularBufferExt<int> buffer_test = {5, 4, 3, 2, 1};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, PopTest1) {
    CircularBufferExt<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_back();

    CircularBufferExt<int> buffer_test = {1, 2, 3, 4};
    ASSERT_EQ(buffer, buffer_test);
    ASSERT_TRUE(buffer.size() == 4);
}

TEST(ExtendedCircularBufferTest, PopTest2) {
    CircularBufferExt<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_back();
    buffer.pop_back();
    buffer.pop_back();

    CircularBufferExt<int> buffer_test = {1, 2};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, PopTest3) {
    CircularBufferExt<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_front();
    buffer.pop_front();
    buffer.pop_front();

    CircularBufferExt<int> buffer_test = {4, 5};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, PopTest4) {
    CircularBufferExt<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_front();
    buffer.pop_back();
    buffer.pop_front();

    CircularBufferExt<int> buffer_test = {3, 4};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, EmplaceTest) {
    CircularBufferExt<int> buffer = {1, 9};
    buffer.reserve(10);
    buffer.emplace(buffer.cbegin() + 1, 8);
    buffer.emplace(buffer.cbegin() + 1, 7);
    buffer.emplace(buffer.cbegin() + 1, 6);
    buffer.emplace(buffer.cbegin() + 1, 5);
    buffer.emplace(buffer.cbegin() + 1, 4);
    buffer.emplace(buffer.cbegin() + 1, 3);
    buffer.emplace(buffer.cbegin() + 1, 2);

    CircularBufferExt<int> buffer_test = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, InsertTest1) {
    CircularBufferExt<int> buffer = {5, 4, 2, 1};
    buffer.reserve(10);
    int value = 3;
    buffer.insert(buffer.cbegin() + 2, value);

    CircularBufferExt<int> buffer_test = {5, 4, 3, 2, 1};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, InsertTest2) {
    CircularBufferExt<int> buffer = {5, 4, 2, 1};
    buffer.reserve(10);
    buffer.insert(buffer.cbegin() + 2, 3);

    CircularBufferExt<int> buffer_test = {5, 4, 3, 2, 1};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, InsertTest3) {
    CircularBufferExt<int> buffer = {1, 1, 1};
    buffer.reserve(10);
    unsigned n = 5;
    int value = 1;
    buffer.insert(buffer.cbegin() + 2, n, value);

    CircularBufferExt<int> buffer_test = {1, 1, 1, 1, 1, 1, 1, 1};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, InsertTest4) {
    std::vector<int> arr = {3, 4, 5};

    CircularBufferExt<int> buffer = {1, 2, 6};
    buffer.reserve(10);
    buffer.insert(buffer.cbegin() + 2, arr.begin(), arr.end());

    CircularBufferExt<int> buffer_test = {1, 2, 3, 4, 5, 6};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, InsertTest5) {
    CircularBufferExt<int> buffer = {1, 2, 6};
    buffer.reserve(10);
    buffer.insert(buffer.cbegin() + 2, {3, 4, 5});

    CircularBufferExt<int> buffer_test = {1, 2, 3, 4, 5, 6};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, EraseTest1) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.reserve(10);
    buffer.erase(buffer.cbegin() + 2);

    CircularBufferExt<int> buffer_test = {1, 2, 4, 5};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, EraseTest2) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.reserve(10);
    buffer.erase(buffer.cbegin());

    CircularBufferExt<int> buffer_test = {2, 3, 4, 5};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, EraseTest3) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.reserve(10);
    buffer.erase(buffer.cbegin() + 2, buffer.cend());

    CircularBufferExt<int> buffer_test = {1, 2};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(ExtendedCircularBufferTest, AssignTest1) {
    CircularBufferExt<int> buffer;
    unsigned n = 5;
    int value = 1;
    buffer.assign(n, value);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 1);
    ASSERT_EQ(buffer[2], 1);
    ASSERT_EQ(buffer[3], 1);
    ASSERT_EQ(buffer[4], 1);

    ASSERT_TRUE(buffer.size() == 5);
}

TEST(ExtendedCircularBufferTest, AssignTest2) {
    CircularBufferExt<int> buffer;
    buffer.assign({5, 4, 3, 2, 1});

    ASSERT_EQ(buffer[0], 5);
    ASSERT_EQ(buffer[1], 4);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 2);
    ASSERT_EQ(buffer[4], 1);

    ASSERT_TRUE(buffer.size() == 5);
}

TEST(ExtendedCircularBufferTest, AssignTest3) {
    std::vector<int> arr = {5, 4, 3, 2, 1};

    CircularBufferExt<int> buffer;
    buffer.assign(arr.begin(), arr.end());

    ASSERT_EQ(buffer[0], 5);
    ASSERT_EQ(buffer[1], 4);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 2);
    ASSERT_EQ(buffer[4], 1);

    ASSERT_TRUE(buffer.size() == 5);
}

TEST(ExtendedCircularBufferTest, ClearTest) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.clear();

    ASSERT_TRUE(buffer.empty());
}

TEST(ExtendedCircularBufferTest, ResizeTest1) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.resize(10, 0);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 4);
    ASSERT_EQ(buffer[4], 5);
    ASSERT_EQ(buffer[5], 0);
    ASSERT_EQ(buffer[6], 0);
    ASSERT_EQ(buffer[7], 0);
    ASSERT_EQ(buffer[8], 0);
    ASSERT_EQ(buffer[9], 0);

    ASSERT_TRUE(buffer.size() == 10);
}

TEST(ExtendedCircularBufferTest, ResizeTest2) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.resize(10);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 4);
    ASSERT_EQ(buffer[4], 5);
    ASSERT_EQ(buffer[5], 0);
    ASSERT_EQ(buffer[6], 0);
    ASSERT_EQ(buffer[7], 0);
    ASSERT_EQ(buffer[8], 0);
    ASSERT_EQ(buffer[9], 0);

    ASSERT_TRUE(buffer.size() == 10);
}

TEST(ExtendedCircularBufferTest, ResizeTest3) {
    CircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.resize(3);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);

    ASSERT_TRUE(buffer.size() == 3);
}