#include <graphics/Text.hpp>


namespace Ruby {
    void Text::Display(u32 x, u32 y) {
       
    }


    bool Text::operator==(const Text& other) { 
        return (m_text == other.m_text) && (m_font == other.m_font); 
    }

    bool Text::operator!=(const Text& other) { 
        return !(*this == other); 
    }


    Text& Text::operator=(const Text& other) {
        if (*this == other)
            return *this;
        
        m_text = other.m_text;
        m_font = other.m_font;

        return *this;
    }

    Text& Text::operator=(const String& msg) {
        m_text = msg;

        return *this;
    }
}