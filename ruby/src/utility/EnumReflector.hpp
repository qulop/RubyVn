#pragma once

#include <types/StdInc.hpp>
#include "Definitions.hpp"


namespace Ruby {
    class EnumReflector {
        using EnumType = Vector<std::pair<String, i32>>;
    public:
        class EnumField {
        public:
            RUBY_NODISCARD i32 GetValue() const;
            RUBY_NODISCARD String GetFieldName() const;
            RUBY_NODISCARD i32 GetIndex() const;
            EnumReflector& GetReflector();

            EnumField& operator++();
            EnumField operator++(int);

            bool operator==(const EnumField& other) const;
            bool operator!=(const EnumField& other) const;

            operator bool() const;  // NOLINT
            RUBY_NODISCARD bool IsHasValue() const;

            const EnumField& operator*() const;

            friend std::ostream& operator<<(std::ostream&, const EnumField&);

        private:
            explicit EnumField(std::shared_ptr<EnumReflector>&& reflector=nullptr, i32 index=-1);

        private:
            friend class EnumReflector;

            const std::shared_ptr<EnumReflector> m_reflector = nullptr;
            i32 m_index = -1;
        };


        EnumReflector(const i32* values, i32 valuesNumber, const char* enumName, const char* strValues);
        EnumReflector(const EnumReflector& other) = default;


        template<typename EnumType>
        static EnumReflector& Create(EnumType e = EnumType{}) { 
            return reflectCreator(e); 
        }

        RUBY_NODISCARD size_t Size() const;

        RUBY_NODISCARD String GetName() const;

        RUBY_NODISCARD EnumField GetByKey(const String& key) const;
        RUBY_NODISCARD EnumField GetByValue(i32 value) const;

        RUBY_NODISCARD EnumField At(i32 i) const;

        RUBY_NODISCARD EnumField begin() const;
        RUBY_NODISCARD EnumField end() const;

    private:
        EnumReflector(EnumReflector&& other);   // NOLINT
        
    private:
        EnumType m_enum;
        String m_enumName;
    };
}

namespace std {
    template<>
    struct formatter<Ruby::EnumReflector::EnumField> {
        template<typename FormatParseContext>
        constexpr auto parse(FormatParseContext& ctx) {   // NOLINT
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const Ruby::EnumReflector::EnumField& field, FormatContext& ctx) const {
            return format_to(ctx.out(), "<{}({})>", field.GetFieldName(), field.GetValue());
        }

    };
}

#define RUBY_ENUM_DETAILS_namespace                                                 \
        extern "C" {}                                                               \
        inline                                                                  

#define RUBY_ENUM_DETAILS_class friend                      

#define RUBY_CREATE_ENUM(enumName, EnumType, IntType, ...)                      \
    enum enumName : IntType                                                     \
    { __VA_ARGS__ };                                                            \
                                                                                \
    RUBY_ENUM_DETAILS_##EnumType Ruby::EnumReflector& reflectCreator(enumName)  \
    {                                                                           \
        static Ruby::EnumReflector reflector{ []() {                            \
            static IntType nextValue;                                           \
            nextValue = 0;                                                      \
                                                                                \
            struct Value {                                                      \
                Value() : val(nextValue) { nextValue += 1; }                    \
                Value(int v) : val(v) { nextValue = val + 1; }                  \
                Value(const Value& other) : val(other) { nextValue = val + 1; } \
                                                                                \
                Value& operator=(const Value&) { return *this; }                \
                Value& operator=(int) { return *this; }                         \
                operator IntType() const { return val; }                        \
                IntType val;                                                    \
            } __VA_ARGS__;                                                      \
                                                                                \
            const IntType values[] = { __VA_ARGS__ };                           \
            IntType valSize = sizeof(values) / sizeof(IntType);                 \
                                                                                \
            return Ruby::EnumReflector(values,                                  \
                                       valSize,                                 \
                                       RUBY_MAKE_STRING(enumName),              \
                                       RUBY_MAKE_STRING((__VA_ARGS__)));        \
            }()                                                                 \
        };                                                                      \
                                                                                \
        return reflector;                                                       \
    }
