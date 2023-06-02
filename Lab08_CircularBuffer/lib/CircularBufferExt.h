#ifndef LABWORK8_CIRCULARBUFFEREXT_H
#define LABWORK8_CIRCULARBUFFEREXT_H

#include "CircularBuffer.h"

template<typename T, typename A = std::allocator<T>>
class CircularBufferExt : public CircularBuffer<T, A> {
protected:
    void increase_size() override {
        this->size_++;
        if (this->size_ > this->capacity_) {
            this->reserve(2 * this->capacity());
        }
    }

public:
    typedef T value_type;               // type in the container
    typedef T& reference;               // ref type in container
    typedef const T& const_reference;   // const ref type in container
    typedef ptrdiff_t difference_type;  // type of diff between iterators
    typedef size_t size_type;           // type of sizes in container

    explicit CircularBufferExt(size_type size = 0) : CircularBuffer<T, A>(size) {}

    CircularBufferExt(const CircularBufferExt& other) : CircularBuffer<T, A>(other) {}

    CircularBufferExt(CircularBufferExt&& other) noexcept: CircularBuffer<T, A>(other) {}

    template<typename iter>
    CircularBufferExt(iter first, iter last) : CircularBuffer<T, A>(first, last) {}

    CircularBufferExt(size_type n, const T& value) : CircularBuffer<T, A>(n, value) {}

    CircularBufferExt(std::initializer_list<T> il) : CircularBuffer<T, A>(il) {}

    ~CircularBufferExt() = default;

    CircularBufferExt& operator=(const CircularBufferExt& other) = default;

    CircularBufferExt& operator=(CircularBufferExt&& other) noexcept = default;
};

#endif //LABWORK8_CIRCULARBUFFEREXT_H
