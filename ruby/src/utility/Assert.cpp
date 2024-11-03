#include "Assert.hpp"
#include <types/StdInc.hpp>


namespace Ruby::Details::Assert {
    String _getAssertionString(const char* expr, const String& msg, std::source_location& loc) {
        auto res = std::format("Assertion failed:\n\t-- Expression: {}\n\t-- File: {}\n\t-- Line: {}\n",
                               expr, loc.file_name(), loc.line());

        if(!msg.empty())
            res += std::format("\t-- Message: {}\n", msg);
        return res;
     }
}