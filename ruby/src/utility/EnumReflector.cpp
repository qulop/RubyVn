#include "EnumReflector.hpp"
#include "RubyUtility.hpp"
#include <types/Logger.hpp>
#include <utility/Assert.hpp>


namespace Ruby {
    using EnumField = EnumReflector::EnumField;
    
    RUBY_FORCEINLINE bool isAllowedChar(char ch) {
        return std::isalpha(ch) || std::isdigit(ch) || ch == '_';
    }

    RUBY_FORCEINLINE bool isEndOfToken(char ch) {
        return ch == ',' || ch == ' ' || ch == ')';
    }

    void skipValueTokens(const char*& str) {
        static i32 nestingLevel = 0;
        loop {
            switch (*str) {
                case '(': ++nestingLevel; break;
                case ')': case ',': {
                    if (nestingLevel == 0)
                        return;
                    else
                        --nestingLevel;
                    break;
                }
            }
            ++str;
        }
    }

    std::optional<String> getField(const char*& str) {
        String field;
        loop {
            char ch = *str;
            RUBY_ASSERT_BASIC(ch);

            if (isEndOfToken(ch))
                break;

            if (!isAllowedChar(ch)) {
                RUBY_ERROR("Details::Enum::_getField() : symbol {} isn't allowed here.", ch);
                return std::nullopt;
            }

            field.push_back(ch);
            ++str;
        }

        return field;
    }

    
    
    
    i32 EnumField::GetValue() const {
        return m_reflector->m_enum.at(m_index).second;
    }

    String EnumField::GetFieldName() const {
        return m_reflector->m_enum.at(m_index).first;
    }

    i32 EnumField::GetIndex() const {
        return m_index;
    }

    EnumReflector& EnumField::GetReflector() {
        return *m_reflector;
    }

    EnumField& EnumField::operator++() {
        m_index = (m_index < m_reflector->Size() - 1) ? ++m_index : -1;
        return *this;
    }

    EnumField EnumField::operator++(int) {
        auto tmp = *this;
        ++(*this);

        return tmp;
    }

    bool EnumField::operator==(const EnumField &other) const {
        return m_index == other.m_index;
    }

    bool EnumField::operator!=(const EnumField& other) const {
        return !(*this == other);
    }

    bool EnumField::IsHasValue() const {
        return m_index != -1;
    }

    EnumField::operator bool() const {
        return IsHasValue();
    }


    const EnumField& EnumField::operator*() const {
        return *this;
    }


    EnumField::EnumField(std::shared_ptr<EnumReflector>&& reflector, i32 index) :
        m_reflector(std::move(reflector)),
        m_index(index)
    {}


    std::ostream& operator<<(std::ostream& os, const EnumField& field) {
        os << "<" << field.GetFieldName() << "(" << field.GetValue() << ")>";

        return os;
    }


    EnumReflector::EnumReflector(const i32* values, i32 valuesNumber, const char* enumName, const char* strValues) :
        m_enumName(enumName)
    {
        RUBY_MAYBE_UNUSED char lastChar = strValues[std::strlen(strValues) - 1];

        RUBY_ASSERT(*strValues == '(' && lastChar == ')',
            "EnumReflector::EnumReflector() : first and last symbols of strValues must be '(' and ')' respectively.");
        ++strValues;

        i32 fieldIndex = 0;
        for (; ; strValues++) {
            RUBY_ASSERT_BASIC(*strValues);
            if (*strValues == ' ')
                continue;

            auto&& fieldName = getField(strValues);
            skipValueTokens(strValues);

            if (!fieldName)
                return;

            m_enum.emplace_back(
                std::make_pair(fieldName.value(), values[fieldIndex]));

            if (*strValues == ')')
                break;

            ++fieldIndex;
        }
    }

    size_t EnumReflector::Size() const {
        return m_enum.size();
    }

    EnumField EnumReflector::At(i32 i) const {
        return (i < m_enum.size()) ? EnumField{ std::make_shared<EnumReflector>(*this), i } :
                                     end();
    }

    String EnumReflector::GetName() const {
        return m_enumName;
    }

    EnumField EnumReflector::GetByKey(const String& key) const {
        for (i32 i = 0; i < m_enum.size(); i++) {
            const auto& [keyInEnum, _] = m_enum.at(i);
            if (keyInEnum == key)
                return At(i);
        }
        return end();
    }

    EnumField EnumReflector::GetByValue(i32 value) const {
        for (i32 i = 0; i < m_enum.size(); i++)
            if (m_enum.at(i).second == value)
                return At(i);
        return end();
    }

    RUBY_NODISCARD EnumField EnumReflector::begin() const {
        return At(0);
    }

    RUBY_NODISCARD EnumField EnumReflector::end() const {
        return EnumField{};
    }
}