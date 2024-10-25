#pragma once

#include <utility/Definitions.hpp>


namespace Ruby::Memory {
    namespace Details {
        constexpr size_t STACK_ALLOC_MAX = 1024 * 16;   // 16kb

        constexpr size_t ALLOC_DEFAULT_POOL_SIZE = 1024 * 4; // 4Kb
    };


    struct AllocatedBlock {
        const size_t size = 0;
        void* ptr = nullptr;

        AllocatedBlock() = default;
        AllocatedBlock(size_t s, void* p) :
            size(s),
            ptr(p)
        {}

        template<typename Tx>
        Tx* GetPtr() {
            return static_cast<Tx*>(ptr);
        }

        bool IsAllocated() const {
            return ptr == nullptr;
        }
    };


    interface IAllocatorTraits {
        virtual AllocatedBlock Allocate(size_t n) = 0;

        void* allocate(size_t n) {
            return Allocate(n).ptr;
        }

        virtual void Deallocate(AllocatedBlock block) = 0;

        void deallocate(void* ptr, size_t n) {
            Deallocate(AllocatedBlock{ n, ptr });
        }
    };
}