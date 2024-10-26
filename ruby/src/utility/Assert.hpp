#pragma once

#include <source_location>

#include <platform/Platform.hpp>
#include <types/TypeTraits.hpp>
#include <types/Singleton.hpp>


namespace Ruby::Details::Assert {
    RubyString _getAssertionString(const char* expr, const RubyString& msg, std::source_location& loc);

    template<typename... Args>
    bool _rubyAssert(const char* expr, std::format_string<Args...> fmt="",
                     std::source_location loc = std::source_location::current(), Args&&... args) {
        RubyString msg = std::format(std::move(fmt), std::forward<Args>(args)...);
        auto res = _getAssertionString(expr, msg, loc);

        Platform::writeInConsole(res.c_str());
        std::abort();

        return false;
    }


    template<typename... Args>
    void _rubyWreck(std::format_string<Args...> fmt, Args&&... args) {
        RubyString msg = std::format(fmt, std::forward<Args>(args)...);

        Platform::writeInConsole(msg.c_str());
        std::abort();
    }
}


#ifndef _NDEBUG
    #define RUBY_ASSERT(expr, fmt, ...)     \
        (static_cast<bool>(expr) || (Ruby::Details::Assert::_rubyAssert(#expr, fmt, std::source_location::current(), __VA_ARGS__)))

    #define RUBY_ASSERT_BASIC(expr)         \
        (static_cast<bool>(expr) || (Ruby::Details::Assert::_rubyAssert(#expr)))
#else
    #define RUBY_ASSERT(expr, msg)
    #define RUBY_ASSERT_BASIC(expr)
#endif


#define RUBY_WRECK(msg, ...)            Ruby::Details::Assert::_rubyWreck(msg, __VA_ARGS__)
#define RUBY_NOT_IMPLEMENTED()          RUBY_WRECK("An unimplemented method(function) was called")
