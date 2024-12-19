#pragma once

#include <utility/Definitions.hpp>

#include "StdInc.hpp"


namespace Ruby {
    namespace ThisThread {
        // ! Only for x86 !
        // Executes the `pause` assembly instruction
        // For more information: https://www.felixcloutier.com/x86/pause
        void cpuPause();
    }


    class SpinWait {
    public:
        SpinWait() = default;
    
        SpinWait(const SpinWait&) = delete;
        SpinWait(SpinWait&&) = delete;
    
        SpinWait& operator=(const SpinWait&) = delete;
        SpinWait& operator=(SpinWait&&) = delete;
    
        void Spin();

        void Reset();

        RUBY_NODISCARD bool IsSpinLimitReached() const;
        RUBY_NODISCARD bool ShouldPark() const;
    
    private:
        static const size_t s_spinLimit = 8;
        static const size_t s_yieldLimit = 16;
    
        size_t m_spinCount = 0;
    };
}