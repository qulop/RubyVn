#pragma once

#include <types/TypeTraits.hpp>


namespace Ruby::Details::Panic {
    template<typename... Args>
    void _runtimePanic(std::format_string<Args...> fmt, std::source_location loc = std::source_location::current(), Args&&... args) {
        String msg = std::format(fmt, std::forward<Args>(args)...);

        std::abort();
    }

#if 0
    consteval void _staticPanic(std::string_view msg) {
        static_assert(Traits::AlwaysFalse<true>::value, msg.data());    // Using of `msg.data` will be possible only in C++26 😓
    }
#endif
}




// Panic while compile-time
#define RUBY_STATIC_PANIC(msg, ...)     Ruby::Details::Panic::_runtimePanic(msg, __VA_OPT__(,) __VA_ARGS__)
#define RUBY_RUNTIME_PANIC(msg, ...)    Ruby::Details::Panic::_runtimePanic(msg, __VA_OPT__(,) __VA_ARGS__)
