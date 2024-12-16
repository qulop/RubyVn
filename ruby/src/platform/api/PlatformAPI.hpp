#pragma once

#include <utility/Definitions.hpp>
#include <types/StdInc.hpp>


namespace Ruby::Platform {
    class Locale {
    public:
        struct LocaleInfo {
            WideString systemLocale;
            WideString systemEncoding;
        };

    public:
        static LocaleInfo GetSystemLocaleInfo(); // Will return struct, which contains `system locale` and `system encoding`
        static Opt<WideString> GetSystemLocale();
        static Opt<WideString> GetSystemEncoding();
    };

    // `width` as `std::pair::first`, and `height` as `std::pair::second`
    std::pair<i32, i32> getScreenResolution() noexcept;

    void errorBox(std::string_view msg, std::string_view title) noexcept;
    void infoBox(std::string_view msg, std::string_view title) noexcept;

    void writeInConsole(std::string_view msg, bool isFlush=true);


    void* virtualAlloc(size_t size);
    void virtualFree(void* address, size_t size=0);
}