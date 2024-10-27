#pragma once

#include "Win32.hpp"

#include <utility/Assert.hpp>


namespace Ruby::Platform {
    std::pair<i32, i32> getScreenResolution() noexcept {
        i32 width = GetSystemMetrics(SM_CXSCREEN);
        i32 height = GetSystemMetrics(SM_CYSCREEN);

        return std::make_pair(width, height);
    }

    void writeInConsole(const RubyString& msg, bool isFlush) {
        HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hnd == NULL || hnd == INVALID_HANDLE_VALUE) {
            errorBox("writeInConsole() : Failed to get STD_OUTPUT_HANDLE", "Terminate");
            std::abort();
        }
        
        DWORD written;
        WriteConsoleA(hnd, msg.c_str(), static_cast<DWORD>(msg.size()), &written, nullptr);
        
        if (isFlush)
            FlushFileBuffers(GetStdHandle(STD_OUTPUT_HANDLE));
    }

    Ptr<wchar_t> getSystemLocale() {
        Ptr<wchar_t> localeStr = std::make_shared<wchar_t>();
        if (GetUserDefaultLocaleName(localeStr.get(), LOCALE_NAME_MAX_LENGTH) == 0)
            return nullptr;

        return localeStr;
    }

    void errorBox(const RubyString& msg, const RubyString& title) noexcept {
        MessageBoxA(nullptr, msg.c_str(), title.c_str(), MB_OK | MB_ICONWARNING | MB_SETFOREGROUND);
    }

    void infoBox(const RubyString& msg, const RubyString& title) noexcept {
        MessageBoxA(nullptr, msg.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
    }

    void* virtualAlloc(size_t size) {
        HANDLE hHeap = GetProcessHeap();
        if (hHeap == static_cast<HANDLE>(NULL))
            return nullptr;

        return HeapAlloc(hHeap, HEAP_ALLOC_FLAGS, size);
    }
    }
}
