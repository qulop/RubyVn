#pragma once

#include <types/TypeTraits.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace Ruby::Math {
    namespace Details::Vector {
        // Aliases for glm vectors
        template<typename Tx>
        using GlmVec2 = glm::vec<2, Tx, glm::defaultp>;

        template<typename Tx>
        using GlmVec3 = glm::vec<3, Tx, glm::defaultp>;

        template<typename Tx>
        using GlmVec4 = glm::vec<4, Tx, glm::defaultp>;
        // ----------------


        template<typename Tx, size_t N>
            requires (std::is_arithmetic_v<Tx> && N <= 4)
        struct _VecBase {
            using ValueType = Tx;

            static constexpr size_t size = N;


            template<typename Head, typename... Tail>
                requires (Ruby::Concepts::HeadAndTailEqual<Head, Tail...> &&
                        sizeof...(Tail) > 0 && sizeof...(Tail) <= 3)
            static constexpr ValueType GetMagnitude_Base(Head&& head, Tail&&... args) {
                if constexpr (std::same_as<Head, f32>)
                    return sqrtf(powf(head) + powf(GetMagnitude_Base(args...)));

                return sqrt(pow(head) + pow(GetMagnitude_Base(args...)));
            }




        };
    

        template<typename Tx>
        class _GeneralVec2 : public _VecBase<Tx, 2> {
            using MyBase = _VecBase<Tx, 2>;

        public:
            using ValueType = Tx;


            union {
                struct { ValueType x, y; };
                struct { ValueType s, t; };
                struct { ValueType r, g; };

                std::array<Tx, MyBase::size> coords = { 0 };
            };

            constexpr _GeneralVec2() :
                x(0), y(0)
            {}

            constexpr _GeneralVec2(ValueType x, ValueType y)  :
                x(x), y(y)
            {}

            explicit constexpr _GeneralVec2(ValueType n) :
                x(n), y(n)
            {}

            constexpr _GeneralVec2(const std::array<Tx, MyBase::size>& coords) :    // NOLINT
                coords(coords)
            {}

            constexpr _GeneralVec2(const GlmVec2<ValueType>& glmVec) {   // NOLINT
                this->x = glmVec.x;
                this->y = glmVec.y;
            }



            constexpr ValueType GetMagnitude() const {
                return MyBase::GetMagnitude_Base(x, y);
            }

            constexpr ValueType GetNormalized() const {
                constexpr auto magnitude = GetMagnitude();

                return _GeneralVec2<ValueType>(x / magnitude, y / magnitude);
            }
        };


        template<typename Tx>
        class _GeneralVec3 : public _VecBase<Tx, 3> {
            using MyBase = _VecBase<Tx, 3>;

        public:
            using ValueType = Tx;

            union {
                struct { ValueType x, y, z; };
                struct { ValueType s, t, p; };
                struct { ValueType r, g, b; };

                std::array<ValueType, MyBase::size> coords = { 0 };
            };
        };


        template<typename Tx>
        class _GeneralVec4 : public _VecBase<Tx, 4> {
            using MyBase = _VecBase<Tx, 3>;

        public:
            using ValueType = Tx;

            union {
                struct { ValueType x, y, z, w; };
                struct { ValueType s, t, p, q; };
                struct { ValueType r, g, b, a; };

                std::array<ValueType, MyBase::size> coords = { 0 };
            };
        };
    }


    using Vec2  = Details::Vector::_GeneralVec2<f32>;
    using IVec2 = Details::Vector::_GeneralVec2<i32>;
    using UVec2 = Details::Vector::_GeneralVec2<u32>;
    using BVec2 = Details::Vector::_GeneralVec2<bool>;
    using DVec2 = Details::Vector::_GeneralVec2<f64>;

    using Vec3  = Details::Vector::_GeneralVec3<f32>;
    using IVec3 = Details::Vector::_GeneralVec3<i32>;
    using UVec3 = Details::Vector::_GeneralVec3<u32>;
    using BVec3 = Details::Vector::_GeneralVec3<bool>;
    using DVec3 = Details::Vector::_GeneralVec3<f64>;

    using Vec4  = Details::Vector::_GeneralVec4<f32>;
    using IVec4 = Details::Vector::_GeneralVec4<i32>;
    using UVec4 = Details::Vector::_GeneralVec4<u32>;
    using BVec4 = Details::Vector::_GeneralVec4<bool>;
    using DVec4 = Details::Vector::_GeneralVec4<f64>;
    
    using Point2D = Vec2;
    using Point3D = Vec3;
}
