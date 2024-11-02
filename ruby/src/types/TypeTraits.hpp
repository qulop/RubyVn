#pragma once

#include <types/StdInc.hpp>


namespace Ruby {
    namespace Traits {
        template<typename Fn, typename... Args>
        struct IsInvocable {

        public:
            static constexpr bool val = true;
        };
    }

    namespace Concepts {
        template<typename Tx>
        concept Iterable = requires(std::ranges::range_value_t<Tx> rng) {
            rng.begin(); rng.end();
            rng.Begin(); rng.End();
        };

        template<typename Fn, typename... Args>
        concept Callable = Traits::IsInvocable<Fn, Args...>::val;


        template<typename Tx>
        concept STLContainterLike = true;
    }

    using RubyString                        = std::string;
    using RubyWideString                    = std::wstring;

    using RubyPath                          = std::filesystem::path;

    template<typename Tx, typename Ty>
    using RubyHashMap                       = std::unordered_map<Tx, Ty>;

    template<typename Tx, typename Allocator=std::allocator<Tx>>
    using RubyVector                        = std::vector<Tx>;

    template<typename Tx>
    using SharedPtr = std::shared_ptr<Tx>;
   
    template<typename Tx>
    using Opt                               = std::optional<Tx>;

    using u8                                = uint8_t;
    using u16                               = uint16_t;
    using u32                               = uint32_t;
    using u64                               = uint64_t;

    using i8                                = int8_t;
    using i16                               = int16_t;
    using i32                               = int32_t;
    using i64                               = int64_t;

    using f32                               = float;
    using f64                               = double;

    using byte                              = u8;
}
