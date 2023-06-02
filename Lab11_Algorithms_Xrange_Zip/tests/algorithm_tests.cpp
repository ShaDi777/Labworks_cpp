#include <gtest/gtest.h>

#include <forward_list>
#include <list>
#include <vector>

#include "lib/algorithm_lib.h"

// all_of - возвращает true, если все элементы диапазона удовлетворяют некоторому предикату. Иначе false
TEST(AlgorithmTest, all_of_test_1) {
    std::stringstream sstr{"0 2 4 6 8"};
    auto begin = std::istream_iterator<int>(sstr);
    auto end = std::istream_iterator<int>();
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_TRUE(algo::all_of(begin, end, pred));
}

TEST(AlgorithmTest, all_of_test_2) {
    std::forward_list<int> v = {1, 3, 5, 7, 9};
    auto pred = [](int x) { return x % 2 == 1; };

    ASSERT_TRUE(algo::all_of(v.begin(), v.end(), pred));
}

TEST(AlgorithmTest, all_of_test_3) {
    std::vector<int> v = {0, 1, 3, 5, 7, 9};
    auto pred = [](int x) { return x % 2 == 1; };

    ASSERT_FALSE(algo::all_of(v.begin(), v.end(), pred));
}

// any_of - возвращает true, если хотя бы один из элементов диапазона удовлетворяет некоторому предикату. Иначе false
TEST(AlgorithmTest, any_of_test_1) {
    std::stringstream sstr{"0 2 4 6 9 8"};
    auto begin = std::istream_iterator<int>(sstr);
    auto end = std::istream_iterator<int>();
    auto pred = [](int x) { return x % 2 == 1; };

    ASSERT_TRUE(algo::any_of(begin, end, pred));
}

TEST(AlgorithmTest, any_of_test_2) {
    std::forward_list<int> v = {1, 3, 5, 7, 9, 2};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_TRUE(algo::any_of(v.begin(), v.end(), pred));
}

TEST(AlgorithmTest, any_of_test_3) {
    std::vector<int> v = {1, 3, 5, 7, 9};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_FALSE(algo::any_of(v.begin(), v.end(), pred));
}


// none_of - возвращает true, если все элементы диапазона не удовлетворяют некоторому предикату. Иначе false
TEST(AlgorithmTest, none_of_test_1) {
    std::stringstream sstr{"0 2 4 6 8"};
    auto begin = std::istream_iterator<int>(sstr);
    auto end = std::istream_iterator<int>();
    auto pred = [](int x) { return x % 2 == 1; };

    ASSERT_TRUE(algo::none_of(begin, end, pred));
}

TEST(AlgorithmTest, none_of_test_2) {
    std::forward_list<int> v = {1, 3, 5, 7, 9};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_TRUE(algo::none_of(v.begin(), v.end(), pred));
}

TEST(AlgorithmTest, none_of_test_3) {
    std::vector<int> v = {1, 3, 5, 7, 9, 2};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_FALSE(algo::none_of(v.begin(), v.end(), pred));
}


// one_of - возвращает true, если ровно один элемент диапазона удовлетворяет некоторому предикату. Иначе false
TEST(AlgorithmTest, one_of_test_1) {
    std::stringstream sstr{"0 1 2 4 6 8"};
    auto begin = std::istream_iterator<int>(sstr);
    auto end = std::istream_iterator<int>();
    auto pred = [](int x) { return x % 2 == 1; };

    ASSERT_TRUE(algo::one_of(begin, end, pred));
}

TEST(AlgorithmTest, one_of_test_2) {
    std::forward_list<int> v = {1, 3, 5, 7, 9, 2};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_TRUE(algo::one_of(v.begin(), v.end(), pred));
}

TEST(AlgorithmTest, one_of_test_3) {
    std::vector<int> v = {1, 2, 3, 5, 7, 9, 2};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_FALSE(algo::one_of(v.begin(), v.end(), pred));
}


// find_not - находит первый элемент, не равный заданному
TEST(AlgorithmTest, find_not_test_1) {
    std::stringstream sstr{"0 1 2 4 6 8"};
    auto begin = std::istream_iterator<int>(sstr);
    auto end = std::istream_iterator<int>();
    auto pred = [](int x) { return x == 6; };

    ASSERT_TRUE(algo::find_not(begin, end, 6) ==
                std::find_if_not(begin, end, pred));
}

TEST(AlgorithmTest, find_not_test_2) {
    std::forward_list<int> v = {1, 3, 5, 6, 7, 9, 2};
    auto pred = [](int x) { return x == 6; };

    ASSERT_TRUE(algo::find_not(v.begin(), v.end(), 6) ==
                std::find_if_not(v.begin(), v.end(), pred));
}

TEST(AlgorithmTest, find_not_test_3) {
    std::vector<int> v = {1, 3, 5, 7, 9, 2};
    auto pred = [](int x) { return x == 6; };

    ASSERT_TRUE(algo::find_not(v.begin(), v.end(), 6) ==
                std::find_if_not(v.begin(), v.end(), pred));
}


// find_backward - находит первый элемент, равный заданному, с конца
TEST(AlgorithmTest, find_backward_test_1) {
    std::forward_list<int> v = {1, 2, 1, 3, 1, 4, 5};

    ASSERT_EQ(algo::find_backward(v.begin(), v.end(), 1), ++(++(++(++v.begin()))));
}

TEST(AlgorithmTest, find_backward_test_2) {
    std::list<int> v = {1, 2, 1, 3, 1, 4, 5, 1};

    ASSERT_EQ(algo::find_backward(v.begin(), v.end(), 1), --v.end());
}

TEST(AlgorithmTest, find_backward_test_3) {
    std::vector<int> v = {1};

    ASSERT_EQ(algo::find_backward(v.begin(), v.end(), 1), v.begin());
}


// is_sorted - возвращает true, если все элементы диапазона находятся в отсортированном порядке относительно некоторого критерия
TEST(AlgorithmTest, is_sorted_test_1) {
    std::forward_list<int> v = {1, 2, 3, 4, 5};

    ASSERT_TRUE(algo::is_sorted(v.begin(), v.end(), std::less<>()));
    ASSERT_FALSE(algo::is_sorted(v.begin(), v.end(), std::greater<>()));
}

TEST(AlgorithmTest, is_sorted_test_2) {
    std::list<int> v = {5, 1, 0, -1, -5};

    ASSERT_TRUE(algo::is_sorted(v.begin(), v.end(), std::greater<>()));
    ASSERT_FALSE(algo::is_sorted(v.begin(), v.end(), std::less<>()));
}

TEST(AlgorithmTest, is_sorted_test_3) {
    std::vector<int> v = {1, 10, 2, 9, 4, 5, -5};

    ASSERT_FALSE(algo::is_sorted(v.begin(), v.end(), std::less<>()));

    std::sort(v.begin(), v.end(), std::less<>());

    ASSERT_TRUE(algo::is_sorted(v.begin(), v.end(), std::less<>()));
}


// is_partitioned - возвращает true, если в диапазоне есть элемент, делящий все элементы на удовлетворяющие и не удовлетворяющие - некоторому предикату. Иначе false.
TEST(AlgorithmTest, is_partitioned_test_1) {
    std::stringstream sstr{"0 0 0 0 2 3 4 5"};
    auto begin = std::istream_iterator<int>(sstr);
    auto end = std::istream_iterator<int>();
    auto pred = [](int x) { return x == 0; };

    ASSERT_TRUE(algo::is_partitioned(begin, end, pred));
}

TEST(AlgorithmTest, is_partitioned_test_2) {
    std::forward_list<int> v = {1, 3, 5};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_TRUE(algo::is_partitioned(v.begin(), v.end(), pred));
}

TEST(AlgorithmTest, is_partitioned_test_3) {
    std::vector<int> v = {1, 10, 2, 9, 4, 5, 0};
    auto pred = [](int x) { return x % 2 == 0; };

    ASSERT_FALSE(algo::is_partitioned(v.begin(), v.end(), pred));

    std::partition(v.begin(), v.end(), pred);

    ASSERT_TRUE(algo::is_partitioned(v.begin(), v.end(), pred));
}


// is_palindrome - возвращает true, если заданная последовательность является палиндромом относительно некоторого условия. Иначе false.
TEST(AlgorithmTest, is_palindrome_test_1) {
    std::list<int> v = {1, 2, 3, 4, 5, 4, 3, 2, 1};

    ASSERT_TRUE(algo::is_palindrome(v.begin(), v.end(), std::equal_to<>()));
}

TEST(AlgorithmTest, is_palindrome_test_2) {
    std::list<int> v = {1, 2, 3, 4, 5};

    ASSERT_FALSE(algo::is_palindrome(v.begin(), v.end(), std::equal_to<>()));
}

TEST(AlgorithmTest, is_palindrome_test_3) {
    std::string v = "PALINDROME__EMORDNILAP";

    ASSERT_TRUE(algo::is_palindrome(v.begin(), v.end(), std::equal_to<>()));
}
