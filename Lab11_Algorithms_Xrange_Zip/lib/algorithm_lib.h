#ifndef LABWORK_11_SHADI777_ALGORITHM_LIB_H
#define LABWORK_11_SHADI777_ALGORITHM_LIB_H

namespace algo {
    template<typename InputIterator, typename T>
    InputIterator find(InputIterator begin, InputIterator end, const T& value) {
        for (; begin != end; ++begin) {
            if (*begin == value) return begin;
        }
        return end;
    }

    //find_not - находит первый элемент, не равный заданному
    template<typename InputIterator, typename T>
    InputIterator find_not(InputIterator begin, InputIterator end, const T& value) {
        for (; begin != end; ++begin) {
            if (*begin != value) return begin;
        }
        return end;
    }

    //find_backward - находит первый элемент, равный заданному, с конца
    template<typename ForwardIterator, typename T>
    ForwardIterator find_backward(ForwardIterator begin, ForwardIterator end, const T& value) {
        ForwardIterator iter = end;
        for (; begin != end; ++begin) {
            if (*begin == value) iter = begin;
        }
        return iter;
    }


    template<typename InputIterator, typename Predicate>
    InputIterator find_if(InputIterator begin, InputIterator end, Predicate p) {
        for (; begin != end; ++begin) {
            if (p(*begin)) return begin;
        }
        return end;
    }

    template<typename InputIterator, typename Predicate>
    InputIterator find_if_not(InputIterator begin, InputIterator end, Predicate p) {
        for (; begin != end; ++begin) {
            if (!p(*begin)) return begin;
        }
        return end;
    }

    //all_of - возвращает true, если все элементы диапазона удовлетворяют некоторому предикату. Иначе false
    template<typename InputIterator, typename Predicate>
    bool all_of(InputIterator begin, InputIterator end, Predicate p) {
        return algo::find_if_not(begin, end, p) == end;
    }

    //any_of - возвращает true, если хотя бы один из элементов диапазона удовлетворяет некоторому предикату. Иначе false
    template<typename InputIterator, typename Predicate>
    bool any_of(InputIterator begin, InputIterator end, Predicate p) {
        return algo::find_if(begin, end, p) != end;
    }

    //none_of - возвращает true, если все элементы диапазона не удовлетворяют некоторому предикату. Иначе false
    template<typename InputIterator, typename Predicate>
    bool none_of(InputIterator begin, InputIterator end, Predicate p) {
        return algo::find_if(begin, end, p) == end;
    }

    //one_of - возвращает true, если ровно один элемент диапазона удовлетворяет некоторому предикату. Иначе false
    template<typename InputIterator, typename Predicate>
    bool one_of(InputIterator begin, InputIterator end, Predicate p) {
        InputIterator i = algo::find_if(begin, end, p);
        return i != end && algo::none_of(++i, end, p);
    }


    //is_sorted - возвращает true, если все элементы диапазона находятся в отсортированном порядке относительно некоторого критерия
    template<typename ForwardIterator, typename Compare>
    bool is_sorted(ForwardIterator begin, ForwardIterator end, Compare comparator) {
        if (begin != end) {
            ForwardIterator next = begin;
            while (++next != end) {
                if (comparator(*next, *begin)) return false;
                begin = next;
            }
        }
        return true;
    }

    //is_partitioned - возвращает true, если в диапазоне есть элемент, делящий все элементы на удовлетворяющие и не удовлетворяющие - некоторому предикату. Иначе false.
    template<typename InputIterator, typename Predicate>
    bool is_partitioned(InputIterator begin, InputIterator end, Predicate p) {
        while (begin != end)
            if (!p(*begin++))
                break;

        while (begin != end)
            if (p(*begin++))
                return false;

        return true;
    }


    //is_palindrome - возвращает true, если заданная последовательность является палиндромом относительно некоторого условия. Иначе false.
    template<typename BidirectionalIterator, typename Compare>
    bool is_palindrome(BidirectionalIterator begin, BidirectionalIterator end, Compare comparator) {
        if (begin == end) return true;
        while (begin != end && begin != --end)
            if (!comparator(*(begin++), *end))
                return false;
        return true;
    }
}


#endif //LABWORK_11_SHADI777_ALGORITHM_LIB_H
