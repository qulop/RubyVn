#pragma once

#if defined(RUBY_WIN32_USED)
    #include "win32/Win32.hpp"
#elif defined(RUBY_LINUX_USED)
    #include "linux/Linux.hpp"
#endif

namespace Ruby {
    enum WindowVendor {
        VENDOR_UNKNOWN,
        VENDOR_GLFW,
        VENDOR_WIN32
    };

    template<typename... Args>
    void errorBoxF(std::format_string<Args...> fmt, const RubyString& title, Args&&... args) noexcept {
        auto&& msg = std::format(std::move(fmt), std::forward<Args>(args)...);

        Platform::errorBox(msg, title);
    }

    template<typename... Args>
    void writeInConsoleF(std::format_string<Args...> fmt, Args&&... args) {
        RubyString msg = std::format(std::move(fmt), std::forward<Args>(args)...);

        Platform::writeInConsole(msg);
    }
}