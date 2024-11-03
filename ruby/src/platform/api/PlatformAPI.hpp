#pragma once

#include <types/StdInc.hpp>
#include <utility/Definitions.hpp>


namespace Ruby::Platform {
    using threadproc_t = unsigned long(*)(void*);

    struct LocaleInfo {
        std::wstring_view systemLocale;
        std::wstring_view systemEncoding;
    };


    // <width> as "std::pair::first", and <height> as "std::pair::second"
    std::pair<i32, i32> getScreenResolution() noexcept;

    void errorBox(const String& msg, const String& title) noexcept;
    void infoBox(const String& msg, const String& title) noexcept;

    void writeInConsole(const String& msg, bool isFlush=true);

    LocaleInfo getSystemLocaleInfo();

    SharedPtr<wchar_t> getSystemLocale();
    SharedPtr<wchar_t> getSystemEncoding();

    void* virtualAlloc(size_t size);
    void virtualFree(void* address, size_t size=0);

    void* createThread(threadproc_t procedure, void* arg, size_t stackSize=0, bool suspended=false, unsigned long* threadId=nullptr);
    bool suspendThread(void* thread);
    bool resumeThread(void* thread);
    bool terminateThread(void* thread, unsigned long reason = EXIT_SUCCESS);
    bool setThreadPriority(void* thread, i32 priority);
}