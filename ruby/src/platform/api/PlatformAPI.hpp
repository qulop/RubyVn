#pragma once

#include <types/StdInc.hpp>
#include <utility/Definitions.hpp>


namespace Ruby::Platform {
    // Will return X as first and Y as second
    std::pair<i32, i32> getScreenResolution() noexcept;

    void errorBox(const RubyString& msg, const RubyString& title) noexcept;
    void infoBox(const RubyString& msg, const RubyString& title) noexcept;

    void writeInConsole(const RubyString& msg, bool isFlush=true);

    Ptr<wchar_t> getSystemLocale();
    Ptr<wchar_t> getSystemEncoding();

    void* virtualAlloc(void* address, size_t len, size_t alignment);
    void virtualFree(void* address, size_t len=0);
}