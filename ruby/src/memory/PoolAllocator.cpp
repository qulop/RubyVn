#include "PoolAllocator.hpp"

#include <types/Logger.hpp>
#include <platform/Platform.hpp>


namespace Ruby::Memory {
    // namespace Details {
    //         MemoryPool::MemoryPool(size_t poolSize, size_t blockSize) {
    //             m_begin = malloc(poolSize);
    //         }

    //         void* MemoryPool::Take(size_t n) {

    //         }

    //         void MemoryPool::PutBack(void* ptr, size_t n) {

    //         }

    //         MemoryPool::~MemoryPool() {
    //             free(m_begin);
    //         }
    // }


    PoolAllocator::PoolAllocator(size_t chunkSize, size_t numOfChunks) :
        m_chunkSize(chunkSize),
        m_numOfChunks(numOfChunks)
    {
        m_memory = (byte*)std::malloc(chunkSize * numOfChunks); // TODO: Replace malloc() to mmap()/VirtualAlloc()
        m_metadata.resize(numOfChunks);
    }

    void* PoolAllocator::Allocate(size_t n) {
        std::lock_guard guard{ m_mutex };(std::mutex);

    }

    void PoolAllocator::Deallocate(void* ptr, size_t n) {
        RUBY_MAYBE_UNUSED bool isPtrInRange = ((byte*)ptr >= m_memory) && ((byte*)ptr <= GetEndOfAllocatedMemory());
        RUBY_ASSERT(isPtrInRange, "Pointer is out of allocated memory range!");
        
        std::lock_guard guard{ m_mutex };(std::mutex);

    }


    PoolAllocator& PoolAllocator::operator=(const PoolAllocator& other) {
        if (*this == other)
            return *this;

        std::lock_guard guard{ m_mutex };(std::mutex);
        m_memory = other.m_memory;
        m_metadata = other.m_metadata;
        m_chunkSize = other.m_chunkSize;
        m_numOfChunks = other.m_numOfChunks;

        return *this;
    }

    PoolAllocator& PoolAllocator::operator=(PoolAllocator&& other) {
        if (*this == other)
            return *this;

        std::lock_guard guard{ m_mutex };(std::mutex);
        m_memory = std::exchange(other.m_memory, nullptr);
        m_metadata = std::move(other.m_metadata);
        m_chunkSize = std::exchange(other.m_chunkSize, 0);
        m_numOfChunks = std::exchange(other.m_numOfChunks, 0);

        return *this;
    }

    bool PoolAllocator::operator==(const PoolAllocator& other) {
        return (m_memory == other.m_memory);
    }

    bool PoolAllocator::operator!=(const PoolAllocator& other) {
        return !(*this == other);
    }

    PoolAllocator::~PoolAllocator() {
        delete[] m_memory;
    }


    byte* PoolAllocator::GetEndOfAllocatedMemory() {
        return (m_memory + (m_numOfChunks * m_chunkSize));
    }

}