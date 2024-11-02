#pragma once

#include "Linux.hpp"

#include <types/Logger.hpp>
#include <X11/Xlib.h>


namespace Ruby::Platform {
    std::pair<i32, i32> getScreenResolution() noexcept {
        Display* display = XOpenDisplay(NULL);
        Screen* screen = DefaultScreenOfDisplay(display);

        auto res = std::make_pair(WidthOfScreen(screen), HeightOfScreen(screen));
        XCloseDisplay(display);

        return res;
    }

    void errorBox(const char* msg, const char* title) noexcept {
        RUBY_NOT_IMPLEMENTED();
    }

    void infoBox(const char* msg, const char* title) noexcept {
        RUBY_NOT_IMPLEMENTED();
    }

    void writeInConsole(const RubyString& msg, bool isFlush) {
        RUBY_NOT_IMPLEMENTED();
    }

    SharedPtr<wchar_t> getSystemLocale() {
        RUBY_NOT_IMPLEMENTED();
    }

    void* virtualAlloc(void* address, size_t len, size_t alignment) {
        RUBY_NOT_IMPLEMENTED();

        // void* mapping = mmap(address,
        //                      len,
        //                      PROT_READ | PROT_WRITE,
        //                      MAP_ANONYMOUS | MAP_ALIGNED(alignment),
        //                      -1,
        //                      0);

        // if (mapping == MAP_FAILED) {
        //     RUBY_ERROR("Platform::virtualAlloc(){ mmap() } : Failed to create memory mapping: {}", strerror_r(errno));
        //     return nullptr;
        // }

        // return mapping;
    }

    void virtualFree(void* address, size_t len) {
        RUBY_NOT_IMPLEMENTED();
    }
}