#pragma once

#include <types/StdInc.hpp>


namespace Ruby::Numeric {
    template<typename Tx>
    constexpr auto pow2(Tx&& val) {
        return std::pow(val, 2);
    }

    template<typename Tx>
    constexpr auto pow3(Tx&& val) {
        return std::pow(val, 3);
    }

    template<typename Head, typename... Tail>
        requires std::conjunction_v<std::is_same<Head, Tail...>> && std::is_arithmetic_v<Head>
    constexpr auto accumulateVariadic(Head&& head, Tail&&... tail) {
        if constexpr (sizeof...(Tail) > 0)
            return head + accumulateVariadic(tail...);
        else
            return head;
    }
}