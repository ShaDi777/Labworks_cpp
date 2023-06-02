#ifndef LABWORK_11_SHADI777_XRANGE_H
#define LABWORK_11_SHADI777_XRANGE_H

#include <iostream>

template<typename T>
class xrange {
public:
    class Iterator {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::input_iterator_tag;

        Iterator() : current_(nullptr), iter_step_(T()), has_iter_step_(false) {}

        Iterator(value_type value, value_type st, bool has_step_) : current_(value), iter_step_(st), has_iter_step_(has_step_) {}

        Iterator(const Iterator& other) : current_(other.current_), iter_step_(other.iter_step_), has_iter_step_(other.has_iter_step_) {}

        Iterator& operator=(const Iterator& other) {
            current_ = other.current_;
            iter_step_ = other.step;
            has_iter_step_ = other.has_iter_step_;
            return *this;
        }

        Iterator& operator++() {
            if (has_iter_step_) current_ += iter_step_;
            else current_++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }

        reference operator*() const {
            return current_;
        }

        pointer operator->() const {
            return &current_;
        }

        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const {
            if (iter_step_ > T() || !has_iter_step_) {
                return current_ != other.current_ && current_ < other.current_;
            } else {
                return current_ != other.current_ && current_ > other.current_;
            }
        }

    private:
        value_type current_;
        value_type iter_step_;
        bool has_iter_step_;
    };

    explicit xrange(T end) : begin_(T()), end_(end) {}

    xrange(T begin, T end) : begin_(begin), end_(end) {}

    xrange(T begin, T end, T step) : begin_(begin), end_(end), step_(step), has_step_(true) {
        if ((begin + step) == begin) throw std::invalid_argument("arg 3 (step) must not be zero");
    }

    Iterator begin() const {
        return Iterator(begin_, step_, has_step_);
    }

    Iterator end() const {
        return Iterator(end_, step_, has_step_);
    }

private:
    T begin_;
    T end_;
    T step_;
    bool has_step_ = false;
};

#endif //LABWORK_11_SHADI777_XRANGE_H
