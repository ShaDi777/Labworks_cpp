#include <gtest/gtest.h>

#include "lib/CircularBuffer.h"


TEST(CircularBufferTest, EmptyTest1) {
    CircularBuffer<std::string> buffer;
    //CircularBuffer<int> buffer;

    ASSERT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, EmptyTest2) {
    CircularBuffer<std::string> buffer(10);

    ASSERT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, EqualityTest1) {
    CircularBuffer<std::string> buffer1 = {"1", "2", "3", "4"};
    CircularBuffer<std::string> buffer2 = {"1", "2", "3", "4"};

    ASSERT_TRUE(buffer1 == buffer2);
}

TEST(CircularBufferTest, EqualityTest2) {
    CircularBuffer<std::string> buffer1 = {"1", "2", "3", "4"};
    CircularBuffer<std::string> buffer2 = {"1", "2", "3", "4", "5"};

    ASSERT_FALSE(buffer1 == buffer2);
}

TEST(CircularBufferTest, ConstructorsTest1) {
    std::vector<int> arr = {1, 2, 3, 4, 5};

    CircularBuffer<int> buffer_iter(arr.begin(), arr.end());
    CircularBuffer<int> buffer_initlist = {1, 2, 3, 4, 5};

    ASSERT_TRUE(buffer_initlist == buffer_iter);
}

TEST(CircularBufferTest, ConstructorsTest2) {
    unsigned n = 5;
    int value = 1;
    CircularBuffer<int> buffer_ncopies(n, value);

    CircularBuffer<int> buffer_copy(buffer_ncopies);

    ASSERT_TRUE(buffer_copy == buffer_ncopies);
}

TEST(CircularBufferTest, ConstructorsTest3) {
    std::vector<int> arr = {1, 1, 1, 1, 1};
    CircularBuffer<int> buffer_iter(arr.begin(), arr.end());

    CircularBuffer<int> buffer_initlist = {1, 1, 1, 1, 1};

    unsigned n = 5;
    int value = 1;
    CircularBuffer<int> buffer_ncopies(n, value);

    CircularBuffer<int> buffer_copy(buffer_iter);


    ASSERT_EQ(buffer_iter, buffer_initlist);
    ASSERT_EQ(buffer_iter, buffer_ncopies);
    ASSERT_EQ(buffer_iter, buffer_copy);
    ASSERT_EQ(buffer_initlist, buffer_ncopies);
    ASSERT_EQ(buffer_initlist, buffer_copy);
    ASSERT_EQ(buffer_ncopies, buffer_copy);
}

TEST(CircularBufferTest, OperatorAssginTest1) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    CircularBuffer<int> buffer_test = {5, 4, 3, 2, 1};
    buffer = {5, 4, 3, 2, 1};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, OperatorAssginTest2) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    CircularBuffer<int> buffer_test = {5, 4, 3, 2, 1};
    buffer = buffer_test;

    ASSERT_TRUE(buffer == buffer_test);
}

TEST(CircularBufferTest, OperatorAssginTest3) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    CircularBuffer<int> buffer_test = {5, 4, 3, 2, 1};
    buffer = CircularBuffer<int>({5, 4, 3, 2, 1});

    ASSERT_TRUE(buffer == buffer_test);
}

TEST(CircularBufferTest, EmplaceFrontTest1) {
    CircularBuffer<int> buffer = {1, 2, 3};
    buffer.reserve(10);
    buffer.emplace_front(0);
    buffer.emplace_front(-1);
    buffer.emplace_front(-2);
    buffer.emplace_front(-3);

    CircularBuffer<int> buffer_test = {-3, -2, -1, 0, 1, 2, 3};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, PushTest1) {
    CircularBuffer<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    CircularBuffer<int> buffer_test = {3, 4, 5};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, PushTest2) {
    CircularBuffer<int> buffer = {3, 2, 1};
    buffer.push_front(4);
    buffer.push_front(5);

    CircularBuffer<int> buffer_test = {5, 4, 3};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, PushTest3) {
    CircularBuffer<int> buffer = {1, 2, 3};
    buffer.reserve(10);
    buffer.push_back(4);
    buffer.push_back(5);

    CircularBuffer<int> buffer_test = {1, 2, 3, 4, 5};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, PushTest4) {
    CircularBuffer<int> buffer = {3, 2, 1};
    buffer.reserve(10);
    buffer.push_front(4);
    buffer.push_front(5);

    CircularBuffer<int> buffer_test = {5, 4, 3, 2, 1};
    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, PopTest1) {
    CircularBuffer<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_back();

    CircularBuffer<int> buffer_test = {3, 4};
    ASSERT_EQ(buffer, buffer_test);
    ASSERT_TRUE(buffer.size() == 2);
}

TEST(CircularBufferTest, PopTest2) {
    CircularBuffer<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_back();
    buffer.pop_back();
    buffer.pop_back();

    ASSERT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, PopTest3) {
    CircularBuffer<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_front();
    buffer.pop_front();
    buffer.pop_front();

    ASSERT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, PopTest4) {
    CircularBuffer<int> buffer = {1, 2, 3};
    buffer.push_back(4);
    buffer.push_back(5);

    buffer.pop_front();
    buffer.pop_back();
    buffer.pop_front();

    ASSERT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, EmplaceTest) {
    CircularBuffer<int> buffer = {1, 9};
    buffer.reserve(10);
    buffer.emplace(buffer.cbegin() + 1, 8);
    buffer.emplace(buffer.cbegin() + 1, 7);
    buffer.emplace(buffer.cbegin() + 1, 6);
    buffer.emplace(buffer.cbegin() + 1, 5);
    buffer.emplace(buffer.cbegin() + 1, 4);
    buffer.emplace(buffer.cbegin() + 1, 3);
    buffer.emplace(buffer.cbegin() + 1, 2);

    CircularBuffer<int> buffer_test = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, InsertTest1) {
    CircularBuffer<int> buffer = {5, 4, 2, 1};
    buffer.reserve(10);
    int value = 3;
    buffer.insert(buffer.cbegin() + 2, value);

    CircularBuffer<int> buffer_test = {5, 4, 3, 2, 1};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, InsertTest2) {
    CircularBuffer<int> buffer = {5, 4, 2, 1};
    buffer.reserve(10);
    buffer.insert(buffer.cbegin() + 2, 3);

    CircularBuffer<int> buffer_test = {5, 4, 3, 2, 1};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, InsertTest3) {
    CircularBuffer<int> buffer = {1, 1, 1};
    buffer.reserve(10);
    unsigned n = 5;
    int value = 1;
    buffer.insert(buffer.cbegin() + 2, n, value);

    CircularBuffer<int> buffer_test = {1, 1, 1, 1, 1, 1, 1, 1};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, InsertTest4) {
    std::vector<int> arr = {3, 4, 5};

    CircularBuffer<int> buffer = {1, 2, 6};
    buffer.reserve(10);
    buffer.insert(buffer.cbegin() + 2, arr.begin(), arr.end());

    CircularBuffer<int> buffer_test = {1, 2, 3, 4, 5, 6};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, InsertTest5) {
    CircularBuffer<int> buffer = {1, 2, 6};
    buffer.reserve(10);
    buffer.insert(buffer.cbegin() + 2, {3, 4, 5});

    CircularBuffer<int> buffer_test = {1, 2, 3, 4, 5, 6};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, EraseTest1) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    buffer.reserve(10);
    buffer.erase(buffer.cbegin() + 2);

    CircularBuffer<int> buffer_test = {1, 2, 4, 5};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, EraseTest2) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    buffer.reserve(10);
    buffer.erase(buffer.cbegin());

    CircularBuffer<int> buffer_test = {2, 3, 4, 5};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, EraseTest3) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    buffer.reserve(10);
    buffer.erase(buffer.cbegin() + 2, buffer.cend());

    CircularBuffer<int> buffer_test = {1, 2};

    ASSERT_EQ(buffer, buffer_test);
}

TEST(CircularBufferTest, AssignTest1) {
    CircularBuffer<int> buffer;
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

TEST(CircularBufferTest, AssignTest2) {
    CircularBuffer<int> buffer;
    buffer.assign({5, 4, 3, 2, 1});

    ASSERT_EQ(buffer[0], 5);
    ASSERT_EQ(buffer[1], 4);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 2);
    ASSERT_EQ(buffer[4], 1);

    ASSERT_TRUE(buffer.size() == 5);
}

TEST(CircularBufferTest, AssignTest3) {
    std::vector<int> arr = {5, 4, 3, 2, 1};

    CircularBuffer<int> buffer;
    buffer.assign(arr.begin(), arr.end());

    ASSERT_EQ(buffer[0], 5);
    ASSERT_EQ(buffer[1], 4);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 2);
    ASSERT_EQ(buffer[4], 1);

    ASSERT_TRUE(buffer.size() == 5);
}

TEST(CircularBufferTest, ClearTest) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    buffer.clear();

    ASSERT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, ResizeTest1) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
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

TEST(CircularBufferTest, ResizeTest2) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
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

TEST(CircularBufferTest, ResizeTest3) {
    CircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    buffer.resize(3);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);

    ASSERT_TRUE(buffer.size() == 3);
}