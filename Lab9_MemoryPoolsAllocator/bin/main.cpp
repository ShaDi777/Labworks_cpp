#include <iostream>
#include <vector>
#include <list>

#include <chrono>
#include <utility>

typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::milliseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()


#include "../lib/MemoryPoolAllocator.h"


template<typename T, typename A = std::allocator<T>>
void func(std::vector<T, A>& vec) {
    for (size_t i = 0; i < 10000000; i++) {
        vec.emplace_back(T());
    }
}

template<typename T, typename A = std::allocator<T>>
void func(std::list<T, A>& lst) {
    for (size_t i = 0; i < 10000000; i++) {
        lst.emplace_back(T());
    }
}

template<typename... Args>
double funcTime(Args&& ... args) {
    TimeVar t1 = timeNow();
    func(std::forward<Args>(args)...);
    return duration(timeNow() - t1);
}

// ===== VECTOR_TEST =====
const uint64_t vec_num_pools = 25;
uint64_t vec_chunk_sizes[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024,
                              2048, 4096, 8192, 16384, 32768, 65536, 131072,
                              262144, 524288, 1048576, 2097152, 4194304,
                              8388608, 16777216};
uint64_t vec_chunk_counts[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1};

void TestVector() {
    std::cout << "\n===== VECTOR =====\n";
    std::vector<char, MemoryPoolAllocator<char, vec_num_pools, vec_chunk_sizes, vec_chunk_counts>> vec_mempool;
    std::vector<char> vec_default;

    std::cout << "Memory_Pool_Allocator = " << funcTime(vec_mempool) << std::endl;
    std::cout << "STD_Allocator = " << funcTime(vec_default) << std::endl;
}

// ====== LIST_TEST =====
const uint64_t list_num_pools = 1;
uint64_t list_chunk_sizes[] = {24};
uint64_t list_chunk_counts[] = {10000000};

void TestList() {
    std::cout << "\n===== LIST_CHAR =====\n";
    std::list<char, MemoryPoolAllocator<char, list_num_pools, list_chunk_sizes, list_chunk_counts>> list_mempool;
    std::list<char> list_default;

    std::cout << "Memory_Pool_Allocator = " << funcTime(list_mempool) << std::endl;
    std::cout << "STD_Allocator = " << funcTime(list_default) << std::endl;
}

int main() {
    TestVector();
    TestList();

    return 0;
}
