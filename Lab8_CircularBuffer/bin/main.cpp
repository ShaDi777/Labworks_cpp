#include <iostream>
#include <vector>
#include <list>
#include "../lib/CircularBuffer.h"
#include "../lib/CircularBufferExt.h"


template<typename T>
void print_debug(T& buffer) {
    std::cout << "Size: " << buffer.size() << '\n';
    std::cout << "Capacity: " << buffer.capacity() << '\n';

    auto iter = buffer.begin();
    for (; iter != buffer.end(); iter++) {
        std::cout << *iter << ' ';
    }
    std::cout << "\n=================\n";
}

int main() {
    CircularBufferExt<int> buffer = {1,2,3};
    buffer = {1,4,5};

    for (auto x : buffer) {
        std::cout << x << ' ';
    }

    /*
    CircularBuffer<int> buffer = {5,9,0,1,3,6,-10, 4};
    buffer.push_back(-99);
    buffer.push_back(-199);
    std::sort(buffer.begin(), buffer.end());
    for (auto& x: buffer) {
        std::cout << x << ' ';
    }
    */

   /*
    CircularBuffer<int> buffer = {0, 1, 2};
    buffer.reserve(10);
    print_debug(buffer);

    for (auto& x: buffer) {
        std::cout << x << ' ';
    }
    */

    return 0;
}

/*
// Buffer of vectors test
int main() {
    CircularBuffer<std::vector<int>> buffer(5);
    buffer.emplace_back(std::vector<int>(1));
    buffer.emplace_back(std::vector<int>(1));
    buffer.emplace_back(std::vector<int>(1));

    buffer[0].emplace_back(1);
    buffer[0].emplace_back(2);

    buffer[2].emplace_back(10);
    buffer[2].emplace_back(100);

    for (auto& v : buffer) {
        for (auto elem : v) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << buffer.capacity();
    return 0;
}
*/