#ifndef LABWORK8_CIRCULARBUFFER_H
#define LABWORK8_CIRCULARBUFFER_H

template<typename T, typename A = std::allocator<T>>
class CircularBuffer {
protected:
    virtual void increase_size() {
        size_++;
        if (size_ > capacity_) {
            size_ = capacity_;
        }
    }

    void circulate(size_t& n) {
        n %= (capacity_ + 1);
        if (n < 0) n += (capacity_ + 1);
    };

public:
    typedef T value_type;               // type in the container
    typedef T& reference;               // ref type in container
    typedef const T& const_reference;   // const ref type in container
    typedef ptrdiff_t difference_type;  // type of diff between iterators
    typedef size_t size_type;           // type of sizes in container

    class iterator;

    class const_iterator {
    public:
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T& reference;
        typedef const T* pointer;
        typedef std::random_access_iterator_tag iterator_category;

        friend class CircularBuffer<T, A>;

        friend class iterator;

        size_type get_cursor() { return cursor; }

        const_iterator(const CircularBuffer* ptr_container, size_type n) : container(ptr_container), cursor(n) {}

        explicit const_iterator() : container(nullptr), cursor(0) {}

        const_iterator(const const_iterator& other) : container(other.container), cursor(other.cursor) {}

        explicit const_iterator(const iterator& other) : container(other.container), cursor(other.cursor) {}

        ~const_iterator() = default;

        const_iterator& operator=(const const_iterator& other) {
            if (this != &other) {
                this->container = other.container;
                this->cursor = other.cursor;
            }
            return *this;
        }

        // Comparison operators
        bool operator==(const const_iterator& other) const {
            return (container->data_ + cursor) == (other.container->data_ + other.cursor);
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

        bool operator<(const const_iterator& other) const {
            return ((other.container->data_ + other.cursor) - (container->data_ + cursor)) > 0;
        }

        bool operator>(const const_iterator& other) const {
            return (other < *this);
        }

        bool operator<=(const const_iterator& other) const {
            return !(*this > other);
        }

        bool operator>=(const const_iterator& other) const {
            return !(*this < other);
        }

        // Increment / Decrement
        const_iterator& operator++() {
            ++cursor;
            cursor %= container->capacity_ + 1;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++cursor;
            cursor %= (container->capacity_ + 1);
            return temp;
        }

        const_iterator& operator--() {
            --cursor;
            cursor = (cursor + container->capacity_ + 1) % (container->capacity_ + 1);
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator temp = *this;
            --cursor;
            cursor = (cursor + container->capacity_ + 1) % (container->capacity_ + 1);
            return temp;
        }

        // Arithmetic
        const_iterator& operator+=(difference_type n) {
            cursor += n;
            cursor %= (container->capacity_ + 1);
            return *this;
        }

        const_iterator operator+(difference_type n) const {
            const_iterator temp = *this;
            temp += n;
            return temp;
        }

        friend const_iterator operator+(difference_type n, const const_iterator& other) {
            const_iterator temp = other;
            temp += n;
            return temp;
        }

        const_iterator& operator-=(difference_type n) {
            *this += -n;
            return *this;
        }

        const_iterator operator-(difference_type n) const {
            const_iterator temp = *this;
            temp += -n;
            return temp;
        }

        difference_type operator-(const const_iterator& other) const {
            difference_type s1;
            if (*this >= this->container->cbegin()) {
                s1 = (cursor - this->container->start_);
            } else {
                s1 = this->container->size() - (this->container->end_ - cursor);
            }

            difference_type s2;
            if (other >= other.container->cbegin()) {
                s2 = (other.cursor - other.container->start_);
            } else {
                s2 = other.container->size() - (other.container->end_ - other.cursor);
            }
            return s1 - s2;
        }

        reference operator*() const {
            return *(container->data_ + cursor);
        }

        pointer operator->() const {
            return (container->data_ + cursor);
        }

        reference operator[](difference_type n) const {
            return *(container->data_ + cursor + n);
        }

    protected:
        const CircularBuffer<T>* container;
        size_type cursor;
    };

    class iterator {
    public:
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::random_access_iterator_tag iterator_category;

        friend class CircularBuffer<T, A>;

        friend class const_iterator;

        size_type get_cursor() const { return this->cursor; }

        iterator(const CircularBuffer* ptr_container, size_type n) : container(ptr_container), cursor(n) {}

        explicit iterator() : container(nullptr), cursor(0) {}

        iterator(const iterator& other) : container(other.container), cursor(other.cursor) {}

        explicit iterator(const_iterator& other) : container(other.container), cursor(other.cursor) {}

        ~iterator() = default;

        iterator& operator=(const iterator& other) {
            this->container = other.container;
            this->cursor = other.cursor;
            return *this;
        }

        bool operator==(const iterator& other) const {
            return (this->container->data_ + this->cursor) == (other.container->data_ + other.cursor);
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }


        bool operator<(const iterator& other) const {
            return ((other.container->data_ + other.cursor) - (this->container->data_ + this->cursor)) > 0;
        }

        bool operator>(const iterator& other) const {
            return (other < *this);
        }

        bool operator<=(const iterator& other) const {
            return !(*this > other);
        }

        bool operator>=(const iterator& other) const {
            return !(*this < other);
        }

        iterator& operator++() {
            ++this->cursor;
            this->cursor %= this->container->capacity_ + 1;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++this->cursor;
            this->cursor %= (this->container->capacity_ + 1);
            return temp;
        }

        iterator& operator--() {
            --this->cursor;
            this->cursor = (this->cursor + this->container->capacity_ + 1) % (this->container->capacity_ + 1);
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --this->cursor;
            this->cursor = (this->cursor + this->container->capacity_ + 1) % (this->container->capacity_ + 1);
            return temp;
        }

        iterator& operator+=(difference_type n) {
            this->cursor += n;
            this->cursor %= (this->container->capacity_ + 1);
            return *this;
        }

        iterator operator+(difference_type n) const {
            iterator temp = *this;
            temp += n;
            return temp;
        }

        friend iterator operator+(difference_type n, const iterator& other) {
            iterator temp = other;
            temp += n;
            return temp;
        }

        iterator& operator-=(difference_type n) {
            *this += -n;
            return *this;
        }

        iterator operator-(difference_type n) const {
            iterator temp = *this;
            temp += -n;
            return temp;
        }

        difference_type operator-(iterator other) const {
            difference_type s1;
            if (this->cursor >= this->container->start_) {
                s1 = (this->cursor - this->container->start_);
            } else {
                s1 = this->container->size() - (this->container->end_ - this->cursor);
            }

            difference_type s2;
            if (other.cursor >= other.container->start_) {
                s2 = (other.cursor - other.container->start_);
            } else {
                s2 = other.container->size() - (other.container->end_ - other.cursor);
            }
            return s1 - s2;
        }

        reference operator*() const {
            return *(this->container->data_ + this->cursor);
        }

        pointer operator->() const {
            return (this->container->data_ + this->cursor);
        }

        reference operator[](size_type n) const {
            return *(this->container->data_ + this->cursor + n);
        }

        void swap(iterator& other) {
            iterator temp = *this;

            this->cursor = other.cursor;
            this->container = other.container;

            other = temp;
        }

        friend void swap(iterator& a, iterator& b) {
            a.swap(b);
        }

    protected:
        const CircularBuffer<T>* container;
        size_type cursor;
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit CircularBuffer(size_type size = 0) :
            capacity_(size), size_(size),
            data_(allocator.allocate(size + 1)),
            start_(0), end_(0) {
        for (auto it = this->data_; it != this->data_ + this->size_; ++it) {
            std::construct_at(it, T());
        }
    }

    CircularBuffer(const CircularBuffer& other) :
            capacity_(other.capacity_), size_(other.size_),
            data_(allocator.allocate(other.capacity_ + 1)),
            start_(other.start_), end_(other.end_) {
        auto val = other.begin();
        for (auto it = this->data_; it != this->data_ + this->size_; ++it, ++val) {
            std::construct_at(it, *val);
        }
    }

    CircularBuffer(CircularBuffer&& other) noexcept {
        this->data_ = std::move(other.data_);
        this->size_ = other.size_;
        this->capacity_ = other.capacity_;
        this->start_ = other.start_;
        this->end_ = other.end_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.start_ = 0;
        other.end_ = 0;
    }

    template<typename iter>
    CircularBuffer(iter first, iter last) {
        size_type n = std::distance(first, last);
        this->size_ = n;
        this->capacity_ = n;
        this->data_ = allocator.allocate(n + 1);
        for (auto it = this->data_; it != this->data_ + this->size_; ++it, ++first) {
            std::construct_at(it, *first);
        }

        this->start_ = 0;
        this->end_ = n;
    }

    CircularBuffer(size_type n, const T& value) :
            capacity_(n), size_(n),
            data_(allocator.allocate(n + 1)),
            start_(0), end_(n) {
        for (auto it = this->data_; it != this->data_ + this->size_; ++it) {
            std::construct_at(it, value);
        }
    }

    CircularBuffer(std::initializer_list<T> il) :
            capacity_(il.size()), size_(il.size()),
            data_(allocator.allocate(il.size() + 1)),
            start_(0), end_(il.size()) {
        auto val = il.begin();
        for (auto it = this->data_; it != this->data_ + this->size_; ++it, ++val) {
            std::construct_at(it, *val);
        }
    }

    ~CircularBuffer() {
        if (this->data_) {
            for (auto it = this->data_; it != this->data_ + this->capacity_; ++it) {
                std::destroy_at(it);
            }
            allocator.deallocate(this->data_, this->capacity_);
        }
    }

    CircularBuffer& operator=(const CircularBuffer& other) {
        if (this != &other) {
            this->size_ = other.size_;
            this->capacity_ = other.capacity_;
            this->start_ = other.start_;
            this->end_ = other.end_;

            if (this->data_) {
                for (auto it = this->data_; it != this->data_ + this->capacity_; ++it) {
                    std::destroy_at(it);
                }
                allocator.deallocate(this->data_, this->capacity_);
            }
            this->data_ = allocator.allocate(other.capacity_ + 1);

            std::copy(other.data_, other.data_ + other.size_, this->data_);
        }
        return *this;
    }

    CircularBuffer& operator=(CircularBuffer&& other) noexcept {
        if (this != &other) {
            if (this->data_) {
                for (auto it = this->data_; it != this->data_ + this->capacity_; ++it) {
                    std::destroy_at(it);
                }
                allocator.deallocate(this->data_, this->capacity_);
            }

            this->allocator = std::move(other.allocator);
            this->data_ = std::move(other.data_);
            this->size_ = std::move(other.size_);
            this->capacity_ = std::move(other.capacity_);
            this->start_ = other.start_;
            this->end_ = other.end_;

            other.data_ = nullptr;
        }

        return *this;
    }

    CircularBuffer& operator=(std::initializer_list<T> il) {
        if (this->data_) {
            for (auto it = this->data_; it != this->data_ + this->capacity_; ++it) {
                std::destroy_at(it);
            }
            allocator.deallocate(this->data_, this->capacity_);
        }
        this->capacity_ = il.size();
        this->size_ = il.size();
        this->data_ = allocator.allocate(il.size() + 1);
        this->start_ = 0;
        this->end_ = il.size();

        std::copy(il.begin(), il.end(), this->data_);

        return *this;
    }

    bool operator==(const CircularBuffer& other) const {
        return (this->size() == other.size()) &&
               std::equal(this->begin(), this->end(), other.begin(), other.end());
    }

    bool operator!=(const CircularBuffer& other) const {
        return !(*this == other);
    }

    iterator begin() {
        return iterator(this, start_);
    }

    const_iterator begin() const {
        return const_iterator(this, start_);
    }

    const_iterator cbegin() const {
        return const_iterator(this, start_);
    }

    iterator end() {
        return iterator(this, end_);
    }

    const_iterator end() const {
        return const_iterator(this, end_);
    }

    const_iterator cend() const {
        return const_iterator(this, end_);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(this->end());
    }

    const_reverse_iterator rbegin() const {
        return this->crbegin();
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(this->end());
    }

    reverse_iterator rend() {
        return reverse_iterator(this->begin());
    }

    const_reverse_iterator rend() const {
        return this->crend();
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(this->begin());
    }

    reference front() {
        return *this->begin();
    }

    const_reference front() const {
        return *this->begin();
    }

    reference back() {
        return *(this->end() - 1);
    }

    const_reference back() const {
        return *(this->end() - 1);
    }

    template<class ...Args>
    void emplace_front(Args&& ... args) {
        increase_size();

        start_ = ((start_ == 0) ? capacity_ : start_ - 1);
        if (this->start_ == this->end_) {
            end_ = ((end_ == 0) ? capacity_ : end_ - 1);
        }
        std::destroy_at(this->data_ + this->begin().get_cursor());
        std::construct_at(this->data_ + this->begin().get_cursor(), std::forward<T>(args)...);
    }

    template<class ...Args>
    void emplace_back(Args&& ... args) {
        std::destroy_at(this->data_ + this->end().get_cursor());
        std::construct_at(this->data_ + this->end().get_cursor(), std::forward<T>(args)...);
        increase_size();
        this->end_++;
        circulate(end_);
        if (this->end_ == this->start_) {
            this->start_++;
            circulate(start_);
        }
    }

    void push_front(const T& x) {
        T copy = x;
        emplace_front(copy);
    }

    void push_front(T&& x) {
        emplace_front(x);
    }

    void push_back(const T& x) {
        T copy = x;
        emplace_back(copy);
    }

    void push_back(T&& x) {
        emplace_back(x);
    }

    void pop_front() {
        start_++;
        circulate(start_);
        size_--;
    }

    void pop_back() {
        end_ = ((end_ == 0) ? capacity_ : end_ - 1);
        size_--;
    }

    reference operator[](size_type n) {
        return *(n + this->begin());
    }

    const_reference operator[](size_type n) const {
        return *(n + this->begin());
    }

    reference at(size_type n) {
        if (n >= this->size()) {
            throw std::out_of_range("Index out of range");
        }
        return (*this)[n];
    }

    const_reference at(size_type n) const {
        if (n >= this->size()) {
            throw std::out_of_range("Index out of range");
        }
        return (*this)[n];
    }

    template<class ...Args>
    iterator emplace(const_iterator pos, Args&& ... args) {
        if (pos == cbegin()) {
            emplace_front(args...);
            return begin();
        } else if (pos == cend()) {
            emplace_back(args...);
            return (end() - 1);
        } else {
            size_type n = pos - this->cbegin();
            T* copied_data = new T[this->cend() - pos];
            iterator i = iterator(pos);
            size_type j = 0;
            while (i != this->end()) {
                copied_data[j++] = *(i++);
            }
            this->size_ = n;
            this->end_ = pos.cursor;

            emplace_back(std::forward<T>(args)...);

            for (int k = 0; k < j; k++) {
                emplace_back(copied_data[k]);
            }

            delete[] copied_data;

            return this->begin() + n;
        }
    }

    iterator insert(const_iterator pos_, const T& value) {
        T copy = value;
        return emplace(pos_, std::move(copy));
    }

    iterator insert(const_iterator pos_, T&& value) {
        return emplace(pos_, value);
    }

    iterator insert(const_iterator pos_, size_type n, T& value) {
        T copy = value;
        auto ptr = emplace(pos_, std::move(copy));
        while (--n > 0) {
            emplace(pos_, std::move(copy));
        }
        return ptr;
    }

    template<class iter>
    iterator insert(const_iterator pos_, iter first, iter last) {
        auto return_pos = iterator(pos_);
        auto it = first;
        while (it != last) {
            auto copy = *(it++);
            emplace(pos_++, std::move(copy));
        }
        return return_pos;
    }

    iterator insert(const_iterator pos_, std::initializer_list<T> il) {
        auto return_pos = iterator(pos_);
        auto it = il.begin();
        while (it != il.end()) {
            auto copy = *(it++);
            emplace(pos_++, std::move(copy));
        }
        return return_pos;
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first == this->cbegin() && last == this->cend()) {
            this->clear();
            return this->end();
        } else {
            size_type s = 0;
            iterator i = this->begin();
            iterator skip = iterator(last);
            while (i != (iterator) first) {
                i++;
                s++;
            }
            while (skip != this->end()) {
                *(i++) = *(skip++);
                s++;
            }
            this->end_ = i.cursor;
            this->size_ = s;

            return iterator(last);
        }
    }

    void clear() {
        this->size_ = 0;
        this->start_ = 0;
        this->end_ = 0;
    }

    template<class iter>
    void assign(iter first, iter last) {
        this->size_ = (last - first);
        if (size_ > capacity_) {
            reserve(last - first);
        }

        this->start_ = 0;
        this->end_ = size_;
        iterator i = this->begin();
        while (first != last) {
            *(i++) = *(first++);
        }
    }

    void assign(std::initializer_list<T> il) {
        this->size_ = il.size();
        if (size_ > capacity_) {
            reserve(il.size());
        }

        this->start_ = 0;
        this->end_ = size_;
        iterator i = this->begin();
        for (auto& x: il) {
            *(i++) = x;
        }
    }

    void assign(size_type n, const T& value) {
        this->size_ = n;
        if (size_ > capacity_) {
            reserve(n);
        }

        this->start_ = 0;
        this->end_ = size_;
        iterator i = this->begin();
        while (n > 0) {
            *(i++) = value;
            n--;
        }
    }

    void swap(CircularBuffer& other) {
        CircularBuffer temp = *this;

        this->start_ = other.start_;
        this->end = other.end_;
        this->size_ = other.size_;
        this->capacity_ = other.capacity_;
        this->data_ = other.data_;

        other = temp;
    }

    size_type size() const {
        return this->size_;
    }

    size_type max_size() const {
        return this->capacity_;
    }

    size_type capacity() const {
        return this->capacity_;
    }

    [[nodiscard]] bool empty() const {
        return this->begin() == this->end();
    }

    void reserve(size_type new_cap) {
        if (this->capacity() < new_cap) {
            T* temp = allocator.allocate(new_cap + 1);
            for (size_type i = 0; i < this->size(); i++) {
                std::construct_at(temp + i, this->data_[i]);
            }

            for (auto it = this->data_; it != this->data_ + this->capacity_; ++it) {
                std::destroy_at(it);
            }
            allocator.deallocate(this->data_, this->capacity_);

            this->data_ = temp;
            this->capacity_ = new_cap;
        }
    }

    void resize(size_type new_size, const value_type& value) {
        T copy = value;

        T* temp = allocator.allocate(new_size);
        for (size_type i = 0; i < new_size; i++) {
            if (i < this->size()) {
                std::construct_at(temp + i, this->data_[i]);
            } else {
                std::construct_at(temp + i, copy);
            }
        }

        for (auto it = this->data_; it != this->data_ + this->capacity_; ++it) {
            std::destroy_at(it);
        }
        allocator.deallocate(this->data_, this->capacity_);

        this->data_ = temp;
        this->size_ = new_size;
        this->capacity_ = new_size;
        this->start_ = 0;
        this->end_ = new_size;
    }

    void resize(size_type n) {
        T temp_value = T();
        resize(n, temp_value);
    }

protected:
    A allocator;
    size_type size_{};
    size_type capacity_{};
    size_type start_;
    size_type end_;
    T* data_;
};

template<class T, class A = std::allocator<T>>
void swap(CircularBuffer<T, A>& a, CircularBuffer<T, A>& b) {
    a.swap(b);
}

#endif //LABWORK8_CIRCULARBUFFER_H
