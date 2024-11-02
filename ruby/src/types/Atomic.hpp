#pragma once

#include "StdInc.hpp"


namespace Ruby {
    constexpr auto MEM_ORDER_RELAXED = std::memory_order::relaxed;
    constexpr auto MEM_ORDER_SEQ_CST = std::memory_order::seq_cst;
    constexpr auto MEM_ORDER_ACQUIRE = std::memory_order::acquire;
    constexpr auto MEM_ORDER_RELEASE = std::memory_order::release;
    constexpr auto MEM_ORDER_CONSUME = std::memory_order::consume;
}