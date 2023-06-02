#ifndef LABWORK9_MEMORYPOOLALLOCATOR_H
#define LABWORK9_MEMORYPOOLALLOCATOR_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>


class Chunk {
private:
    uint8_t* data;
    uint64_t size = 0;
    Chunk* next;

public:
    friend class PoolManager;

    Chunk() {
        data = nullptr;
        size = 0;
        next = nullptr;
    }

    explicit Chunk(uint8_t* memory, uint64_t chunk_size) {
        data = memory;
        size = chunk_size;
    }
};


class PoolManager {
private:
    uint64_t max_chunk_size = 0;
    std::map<uint64_t, Chunk*> m_free_chunks;
    std::unordered_map<uint8_t*, Chunk*> m_used_chunks;

public:
    PoolManager() = default;

    ~PoolManager() {
        for (auto& imap: m_free_chunks) {
            Chunk* head = imap.second->next;
            while (head) {
                Chunk* temp = head->next;

                delete head;

                head = temp;
            }
        }
    }

    uint8_t* createPool(uint64_t ChunkSize, uint64_t ChunkCount) {
        if (max_chunk_size < ChunkSize) max_chunk_size = ChunkSize;

        uint8_t* memory = static_cast<uint8_t*>(malloc(ChunkSize * ChunkCount));

        Chunk* head = nullptr;
        for (size_t i = 0; i < ChunkCount; i++) {
            Chunk* chunk = new Chunk(memory + i * ChunkSize, ChunkSize);

            chunk->next = head;
            head = chunk;
        }
        m_free_chunks[ChunkSize] = head;

        return memory;
    }

    uint8_t* allocate(size_t n) {
        if (n > max_chunk_size) {
            std::cout << "\nNo chunk for " << n << " bytes\n";
            throw std::bad_alloc();
        }

        uint8_t* memory = nullptr;

        for (auto& ifree_chunk: m_free_chunks) {
            if (ifree_chunk.first < n) continue;
            if (!ifree_chunk.second) continue;

            Chunk* found_chunk = ifree_chunk.second;
            memory = found_chunk->data;

            m_used_chunks[found_chunk->data] = found_chunk;

            ifree_chunk.second = ifree_chunk.second->next;

            if (max_chunk_size == ifree_chunk.first && !ifree_chunk.second) {
                auto iter = m_free_chunks.rbegin();
                while (iter != m_free_chunks.rend()) {
                    if (!iter->second && max_chunk_size < iter->first) {
                        max_chunk_size = iter->first;
                        break;
                    }
                    iter++;
                }
            }

            break;
        }

        if (!memory) {
            std::cout << "No chunk found!\n";
            std::cout << "\nNo chunk for " << n << " bytes\n";
            throw std::bad_alloc();
        }

        return memory;
    }

    void deallocate(uint8_t* memory, size_t n) {
        Chunk* chunk = m_used_chunks[memory];

        chunk->next = m_free_chunks[chunk->size];
        m_free_chunks[chunk->size] = chunk;
        if (max_chunk_size < chunk->size) max_chunk_size = chunk->size;
    }

    uint64_t max_size() const {
        return max_chunk_size;
    }
};

template<typename T, size_t NumPools, uint64_t* ChunkSizeArray, uint64_t* ChunkCountArray>
class MemoryPoolAllocator {
private:
    std::shared_ptr<PoolManager> pool_manager;

    std::shared_ptr<uint8_t* []> pools_;

public:
    typedef T value_type;

    typedef T* pointer;
    typedef const T* const_pointer;

    typedef void* void_pointer;
    typedef const void* const_void_pointer;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    template<typename U>
    struct rebind {
        typedef MemoryPoolAllocator<U, NumPools, ChunkSizeArray, ChunkCountArray> other;
    };

    explicit MemoryPoolAllocator() {
        pool_manager = std::make_shared<PoolManager>();

        pools_.reset(new uint8_t* [NumPools],
                     [](uint8_t** p) {
                         for (size_t i = 0; i < NumPools; i++) {
                             free(p[i]);
                         }
                         delete[] p;
                     });

        for (size_t i = 0; i < NumPools; i++) {
            pools_.get()[i] = pool_manager->createPool(ChunkSizeArray[i], ChunkCountArray[i]);
        }
    }

    ~MemoryPoolAllocator() = default;

    pointer allocate(size_type n) {
        auto ptr = reinterpret_cast<pointer>(pool_manager->allocate(n * sizeof(T)));
        return ptr;
    }

    pointer allocate(size_type n, const_void_pointer cvp) {
        return allocate(n);
    }

    void deallocate(pointer p, size_type n) {
        pool_manager->deallocate(reinterpret_cast<uint8_t*>(p), n * sizeof(T));
    }

    size_type max_size() const {
        return pool_manager->max_size();
    }

    template<typename U, typename... Args>
    void construct(U* p, Args&& ...args) {
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destruct(U* p) {
        p->~U();
    }

    bool operator==(const MemoryPoolAllocator& other) {
        std::unordered_set<uint8_t*> allocated_1;
        std::unordered_set<uint8_t*> allocated_2;
        for (size_t i = 0; i < NumPools; i++) {
            allocated_1.insert(pools_.get()[i]);
            allocated_2.insert(other.pools_.get()[i]);
        }
        return allocated_1 == allocated_2;
    }

    bool operator!=(const MemoryPoolAllocator& other) {
        return !(*this == other);
    }

    MemoryPoolAllocator(const MemoryPoolAllocator<T, NumPools, ChunkSizeArray, ChunkCountArray>& other) {
        if (this != &other) {
            this->pool_manager = other.pool_manager;
            this->pools_ = other.pools_;
        }
    }

    std::shared_ptr<PoolManager> getPoolManager() const {
        return pool_manager;
    }

    std::shared_ptr<uint8_t* []> getPools() const {
        return pools_;
    }

    template<typename U>
    explicit MemoryPoolAllocator(
            const MemoryPoolAllocator<U, NumPools, ChunkSizeArray, ChunkCountArray>& other) {
        this->pool_manager = other.getPoolManager();
        this->pools_ = other.getPools();
    }
};

#endif //LABWORK9_MEMORYPOOLALLOCATOR_H
