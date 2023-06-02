#include <gtest/gtest.h>

#include <list>
#include <vector>
#include <forward_list>

#include "lib/zip.h"

TEST(ZipTest, test_1) {
    std::vector<std::pair<int, char>> answer = {{1, 'a'},
                                                {2, 'b'},
                                                {3, 'c'},
                                                {4, 'd'}};
    auto iter_check = answer.begin();

    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(ZipTest, test_2) {
    std::vector<std::pair<int, char>> answer = {{1, 'a'},
                                                {2, 'b'},
                                                {3, 'c'},
                                                {4, 'd'}};
    auto iter_check = answer.begin();

    std::list l = {1, 2, 3, 4};
    std::vector v = {'a', 'b', 'c', 'd', 'e'};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(ZipTest, test_3) {
    std::vector<std::pair<int, char>> answer = {};
    auto iter_check = answer.begin();

    std::list<int> l = {};
    std::vector<char> v = {'a', 'b', 'c', 'd', 'e'};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(ZipTest, test_4) {
    std::vector<std::pair<int, char>> answer = {};
    auto iter_check = answer.begin();

    std::list<int> l = {1, 2, 3, 4};
    std::vector<char> v = {};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(ZipTest, test_5) {
    std::vector<std::pair<int, char>> answer = {{1, 'a'}};
    auto iter_check = answer.begin();

    std::list l = {1};
    std::vector v = {'a', 'b'};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(ZipTest, test_6) {
    std::vector<std::pair<int, char>> answer = {{1, 'a'}};
    auto iter_check = answer.begin();

    std::list l = {1, 2};
    std::vector v = {'a'};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(ZipTest, test_7) {
    std::vector<std::pair<int, char>> answer = {{1, 'a'}};
    auto iter_check = answer.begin();

    std::forward_list l = {1, 2};
    std::vector v = {'a'};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(ZipTest, test_8) {
    std::vector<std::pair<int, char>> answer = {};
    auto iter_check = answer.begin();

    std::forward_list<int> l = {};
    std::vector<char> v = {};

    for (auto value: zip(l, v)) {
        ASSERT_EQ(value.first, iter_check->first);
        ASSERT_EQ(value.second, iter_check->second);
        iter_check++;
    }
    ASSERT_EQ(iter_check, answer.end());
}
