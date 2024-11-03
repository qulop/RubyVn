#pragma once

#include "Definitions.hpp"
#include <types/TypeTraits.hpp>
#include <types/Singleton.hpp>
#include "Enum.hpp"


namespace Ruby {
    String extractTextFromFile(std::string_view path);
    Opt<bool> strToBool(const char* str);


    template<typename IntegralType = i32>
    Opt<IntegralType> strToInt(const char* str) {
        int val = 0;
        auto res = std::from_chars(str, str + std::strlen(str), val);

        if (res.ec != std::errc{})
            return std::nullopt;
        return val;
    }

    template<typename Tx, typename... Args>
    SharedPtr<Tx> makeShared(Args&&... args) {
        return std::make_shared<Tx>(std::forward<Args>(args)...);
    }

    template<typename Tx>
    SharedPtr<Tx> makeShared(size_t size) {
        return std::make_shared<Tx>(size);
    }

    template<typename Tx, typename SharedPtr>
    Tx* isInstanceOf(SharedPtr* ptr) {
        return dynamic_cast<Tx*>(ptr);
    }
}