#pragma once

#include <utility/Definitions.hpp>
#include "FontTTF.hpp"


namespace Ruby {
    class RUBY_API Text {
    public:
        Text() = default;
        Text(const String& text);
        Text(const FontTTF& FontTTF);
        Text(const String& text, const FontTTF& FontTTF);
        Text(const String& text, size_t widht, size_t height, const FontTTF& FontTTF);

        void SetDimensions(size_t width, size_t height);
        void Set(const String& text);

        void Display(u32 x, u32 y);

        bool operator==(const Text& other);
        bool operator!=(const Text& other);
        Text& operator=(const Text& other);
        Text& operator=(const String& msg);

    private:
        String m_text;
        FontTTF m_font;
    };
}