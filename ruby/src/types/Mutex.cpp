#include "Mutex.hpp"


namespace Ruby {
    namespace ThisThread {
        void cpuPause() {
            #if defined(RUBY_GCC_USED) || defined(RUBY_CLANG_USED)
                asm volatile("pause\n\t" : : : "memory");
            #elif defined(RUBY_MSVC_USED)
                __asm pause
            #else
                static_assert(false, "Unsupported compiler used!");
            #endif
        }
    }


    void SpinWait::Spin() {
        if (m_spinCount < SpinWait::s_spinLimit) {
            for (size_t i = 0; i < (1 << m_spinCount); i++) {
                ThisThread::cpuPause();
            }
        }
        else {
            std::this_thread::yield();
        }

        ++m_spinCount;
    }

    void SpinWait::Reset() {
        m_spinCount = 0;
    }

    bool SpinWait::IsSpinLimitReached() const {
        return m_spinCount >= SpinWait::s_spinLimit;
    }

    bool SpinWait::ShouldPark() const {
        return m_spinCount >= SpinWait::s_yieldLimit;
    }
}