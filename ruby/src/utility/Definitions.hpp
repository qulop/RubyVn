#pragma once
           
#include <types/TypeTraits.hpp>


// Placeholder
#define RUBY_API

#define RUBY_NODISCARD                  [[nodiscard]]
#define RUBY_MAYBE_UNUSED               [[maybe_unused]]
#define RUBY_FALLTHROUGH                [[fallthrough]]
#define RUBY_NORETURN                   [[noreturn]]

#ifdef _NDEBUG            
    #define RUBY_LOG_LEVEL                   spdlog::level::info
#else
    #define RUBY_LOG_LEVEL                   spdlog::level::debug
#endif

#if defined(RUBY_MSVC_USED)
    #define RUBY_FORCEINLINE            __forceinline
#elif defined(RUBY_GCC_USED) || defined(RUBY_CLANG_USED)
    #define RUBY_FORCEINLINE            __attribute__((always_inline))
#else
    #define RUBY_FORCEINLINE            inline
#endif

#define RUBY_MAKE_STRING(x)             #x
#define RUBY_SWITCH_BOOL(target)        target = !target

// Helper "keywords"
#ifdef interface
    #undef interface
#endif
#define interface                       struct

#define abstract

#define loop                            while (true)
// -----

#define RUBY_UNDEFINED_ID               (-1)
