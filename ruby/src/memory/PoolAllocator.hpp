#pragma once

#include "IAllocatorTraits.hpp"
#include <types/StdInc.hpp>


namespace Ruby::Memory {
    namespace Details {
        class MemoryPool {
        public:


        private:
        };


        class _PoolAllocatorHeader {
            u32 size = 0;   // Size of allocated memory
            bool isFree = true; 
        };
    }


    constexpr size_t POOL_ALLOC_CHUNK_SIZE_DEFAULT = 8;
    constexpr size_t POOL_ALLOC_NUM_OF_CHUNKS_DEFAULT = 512;    // 4096 bytes


    class PoolAllocator : public IAllocatorTraits {
    public:
        PoolAllocator(size_t chunkSize=POOL_ALLOC_CHUNK_SIZE_DEFAULT,
                      size_t numOfChunks=POOL_ALLOC_NUM_OF_CHUNKS_DEFAULT);

        PoolAllocator(const PoolAllocator& other);
        PoolAllocator(PoolAllocator&& other);

        void* Allocate(size_t n) override;
        void Deallocate(void* ptr, size_t n=0) override;

        void Clear();

        PoolAllocator& operator=(const PoolAllocator& other);
        PoolAllocator& operator=(PoolAllocator&& other);

        bool operator==(const PoolAllocator& other);
        bool operator!=(const PoolAllocator& other);

        ~PoolAllocator();

    private:
        byte* GetEndOfAllocatedMemory();

    private:
        std::mutex m_mutex;

        size_t m_chunkSize = 0;
        size_t m_numOfChunks = 0;

        byte* m_memory = nullptr;
        RubyVector<Details::_PoolAllocatorHeader> m_metadata;
    };
}