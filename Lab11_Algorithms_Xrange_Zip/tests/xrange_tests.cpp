#include <gtest/gtest.h>

#include "lib/xrange.h"

TEST(RangeTest, xrange_int_test_1) {
    std::vector<int> answer = {1, 2, 3, 4};
    auto iter_check = answer.begin();

    auto range = xrange(1, 5);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_int_test_2) {
    std::vector<int> answer = {0, 1, 2, 3};
    auto iter_check = answer.begin();

    auto range = xrange(4);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_int_test_3) {
    std::vector<int> answer = {1, 3, 5};
    auto iter_check = answer.begin();

    auto range = xrange(1, 6, 2);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_int_test_4) {
    std::vector<int> answer = {5, 4, 3, 2, 1};
    auto iter_check = answer.begin();

    auto range = xrange(5, 0, -1);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_int_test_5) {
    EXPECT_THROW(xrange(1, 10, 0), std::invalid_argument);
}

TEST(RangeTest, xrange_double_test_1) {
    std::vector<double> answer = {1.5, 2.5, 3.5, 4.5};
    auto iter_check = answer.begin();

    auto range = xrange(1.5, 5.5);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_double_test_2) {
    std::vector<double> answer = {0, 1, 2, 3, 4};
    auto iter_check = answer.begin();

    auto range = xrange(4.5);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_double_test_3) {
    std::vector<double> answer = {1, 2.5, 4, 5.5, 7, 8.5};
    auto iter_check = answer.begin();

    auto range = xrange(1.0, 10.0, 1.5);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_double_test_5) {
    std::vector<double> answer = {10, 8.5, 7, 5.5, 4, 2.5, 1};
    auto iter_check = answer.begin();

    auto range = xrange(10.0, 0.0, -1.5);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}


class Date {
public:
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    Date() : Date(0, 0, 0) {}

    bool operator==(const Date& other) const {
        return (year == other.year) && (month == other.month) && (day == other.day);
    }

    bool operator!=(const Date& other) const {
        return !(*this == other);
    }

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        if (day != other.day) return day < other.day;
        return false;
    }

    bool operator>(const Date& other) const {
        return !(*this < other || *this == other);
    }

    Date operator+(const Date& other) const {
        Date temp = *this;
        temp.year += other.year;
        temp.month += other.month;
        temp.day += other.day;
        return temp;
    }

    Date& operator+=(const Date& other) {
        year += other.year;
        month += other.month;
        day += other.day;
        return *this;
    }

    Date operator++(int) {
        Date temp = *this;
        day++;
        return temp;
    }

    Date& operator++() {
        day++;
        return *this;
    }

public:
    int year;
    int month;
    int day;
};

TEST(RangeTest, xrange_custom_class_test_1) {
    std::vector<Date> answer = {Date(0, 0, 0), Date(1, 1, 1), Date(2, 2, 2), Date(3, 3, 3), Date(4, 4, 4)};
    auto iter_check = answer.begin();

    Date start(0, 0, 0);
    Date end(5, 5, 5);
    Date step(1, 1, 1);

    auto range = xrange(start, end, step);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_custom_class_test_2) {
    std::vector<Date> answer = {Date(4, 4, 4), Date(3, 3, 3), Date(2, 2, 2), Date(1, 1, 1)};
    auto iter_check = answer.begin();

    Date start(4, 4, 4);
    Date end(0, 0, 0);
    Date step(-1, -1, -1);

    auto range = xrange(start, end, step);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_custom_class_test_3) {
    std::vector<Date> answer = {Date(0, 0, 0), Date(0, 0, 1), Date(0, 0, 2), Date(0, 0, 3), Date(0, 0, 4)};
    auto iter_check = answer.begin();

    Date end(0, 0, 5);

    auto range = xrange(end);
    for (auto x: range) {
        ASSERT_EQ(x, *iter_check++);
    }
    ASSERT_EQ(iter_check, answer.end());
}

TEST(RangeTest, xrange_custom_class_test_4) {
    EXPECT_THROW(xrange(Date(0,0,0), Date(1,2,4), Date(0,0,0)), std::invalid_argument);
}
