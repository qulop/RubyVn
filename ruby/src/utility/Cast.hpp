#pragma once

#include <types/TypeTraits.hpp>

#include "Assert.hpp"
#include "Enum.hpp"


namespace Ruby {
    inline Opt<bool> strToBool(const char* str) {
        String lowerStr{ str };
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), tolower);

        return lowerStr == "true";
    }

    inline Opt<f32> strToFloat(const char* str) {
        RUBY_NOT_IMPLEMENTED();

        return nullopt;
    }


    inline Opt<f64> strToDouble(const char* str) {
        RUBY_NOT_IMPLEMENTED();

        return nullopt;
    }


    template<typename IntegralType = i32>
    Opt<IntegralType> strToInt(const char* str) {
        int val = 0;
        auto res = std::from_chars(str, str + std::strlen(str), val);

        if (res.ec != std::errc{})
            return std::nullopt;
        return val;
    }

    template<typename EnumType, typename EnumDerivedType = i32>
    Opt<String> enumValueToString(EnumType en) {
        return EnumReflector::Create<EnumType>()
                .GetByValue(static_cast<EnumDerivedType>(en))
                .GetFieldName();
    }

    template<typename Tx, typename Ptr>
    Tx* isInstanceOf(Ptr* ptr) {
        return dynamic_cast<Tx*>(ptr);
    }

    template<typename Targ, typename Cur>
    constexpr Targ cast(Cur&& cur) {
        return static_cast<Targ>(std::forward<Cur>(cur));
    }
}