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
        concept CString = std::same_as<std::remove_cv_t<Tx>, char*> || 
                                std::same_as<std::remove_cv_t<Tx>, wchar_t*>;

        template<typename Tx>
        concept ImplementsEqualityOp = requires(Tx a, Tx b) {
            { a == b } -> std::convertible_to<bool>;
        };

        template<typename Tx>
        concept ImplementsInequalityOp = requires(Tx a, Tx b) {
            { a != b } -> std::convertible_to<bool>;
        };


        template<typename Tx>
        concept STLContainterLike = true;
    }

    using String                            = std::string;
    using WideString                        = std::wstring;

    using Path                              = std::filesystem::path;

    template<typename Tx, typename Ty>
    using HashMap                           = std::unordered_map<Tx, Ty>;

    template<typename Tx, typename Allocator=std::allocator<Tx>>
    using Vector                            = std::vector<Tx>;

    template<typename Tx>
    using SharedPtr = std::shared_ptr<Tx>;
   
    template<typename Tx>
    using Opt                               = std::optional<Tx>;

    using NullOptType                       = std::nullopt_t;
    constexpr inline NullOptType nullopt    = std::nullopt;

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


    
    template<typename Tx, typename... Args>
    SharedPtr<Tx> makeShared(Args&&... args) {
        return std::make_shared<Tx>(std::forward<Args>(args)...);
    }

    template<typename Tx, typename Deleter>
    SharedPtr<Tx> makeShared(Tx* ptr, Deleter&& deleter) {
        auto ret = SharedPtr<Tx>{ ptr, std::forward<Deleter>(deleter) };
        return ret;
    }

    template<typename Tx>
    SharedPtr<Tx> makeShared(size_t size) {
        return std::make_shared<Tx>(size);
    }
}
