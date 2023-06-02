#ifndef LABWORK_11_SHADI777_ZIP_H
#define LABWORK_11_SHADI777_ZIP_H

#include <iostream>

template<typename InputIt1, typename InputIt2>
class zip_iterator {
public:
    using value_type = std::pair<
            typename std::iterator_traits<InputIt1>::value_type,
            typename std::iterator_traits<InputIt2>::value_type
    >;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::input_iterator_tag;

    zip_iterator(InputIt1 it1, InputIt2 it2, size_t offset, size_t size)
            : start_it1_(it1), start_it2_(it2),
              cur_it1_(it1), cur_it2_(it2),
              offset_(offset), max_offset_(size) {}

    value_type operator*() const { return make_value(*cur_it1_, *cur_it2_); }

    pointer operator->() const { return &make_value(*cur_it1_, *cur_it2_); }

    zip_iterator& operator++() {
        if (offset_ < max_offset_) {
            ++cur_it1_;
            ++cur_it2_;
            ++offset_;
        }
        return *this;
    }

    zip_iterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    friend bool operator==(const zip_iterator& lhs, const zip_iterator& rhs) {
        return lhs.start_it1_ == rhs.start_it1_ && lhs.start_it2_ == rhs.start_it2_ &&
               lhs.offset_ == rhs.offset_ && lhs.max_offset_ == rhs.max_offset_;
    }

    friend bool operator!=(const zip_iterator& lhs, const zip_iterator& rhs) {
        return !(lhs == rhs);
    }

private:
    template<typename T1 = typename std::iterator_traits<InputIt1>::value_type,
            typename T2 = typename std::iterator_traits<InputIt2>::value_type>
    value_type make_value(T1&& t1, T2&& t2) const {
        return std::make_pair(std::forward<T1>(t1), std::forward<T2>(t2));
    }

    InputIt1 start_it1_;
    InputIt2 start_it2_;

    InputIt1 cur_it1_;
    InputIt2 cur_it2_;

    size_t offset_;
    size_t max_offset_;
};

template<typename Container1, typename Container2>
class zip_container {
public:
    using value_type = typename zip_iterator<
            typename Container1::const_iterator,
            typename Container2::const_iterator
    >::value_type;
    using const_iterator = zip_iterator<
            typename Container1::const_iterator,
            typename Container2::const_iterator
    >;

    zip_container(const Container1& container1, const Container2& container2)
            : begin1_(container1.begin()),
              begin2_(container2.begin()) {
        size_ = std::min(std::distance(container1.begin(), container1.end()),
                         std::distance(container2.begin(), container2.end()));
    }

    const_iterator begin() const { return const_iterator(begin1_, begin2_, 0, size_); }

    const_iterator end() const { return const_iterator(begin1_, begin2_, size_, size_); }

private:
    typename Container1::const_iterator begin1_;
    typename Container2::const_iterator begin2_;
    size_t size_;
};

template<typename Container1, typename Container2>
zip_container<Container1, Container2> zip(const Container1& container1, const Container2& container2) {
    return zip_container<Container1, Container2>(container1, container2);
}

#endif //LABWORK_11_SHADI777_ZIP_H
