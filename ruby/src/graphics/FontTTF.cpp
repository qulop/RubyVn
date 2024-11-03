#include "FontTTF.hpp"

#include <platform/Platform.hpp>
#include <utility/Assert.hpp>


namespace Ruby {
    FontTTF::FontTTF(const String& path, u32 height, u32 width) {
        LoadFontTTF(path);
        SetNewDimensions(height, width);
    }


    void FontTTF::LoadFontTTF(const String& path) {
        if (!m_lib && FT_Init_FreeType(&m_lib)) {
                RUBY_CRITICAL("FreeType criritcal error: cannot to initialize a library");
                return;
        }

        if (FT_New_Face(m_lib, path.c_str(), 0, &m_face))
            if (!FetchSystemFontTTF(path))
                return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }


    void FontTTF::SetNewDimensions(u32 width, u32 height) {
        RUBY_ASSERT(m_face != nullptr, "You firstly must load FontTTF(init FreeType library) before setting it's dimensions!");

        FT_Set_Pixel_Sizes(m_face, width, height);
        LoadGlyphs();

        if (m_FontTTFFamily != m_face->family_name) { 
            m_FontTTFFamily = m_face->family_name; 
        }
    }


    std::optional<Glyph> FontTTF::GetGlyph(char ch) const {
        try { 
            return m_chars.at(ch); 
        }
        catch(std::out_of_range&) { 
            return std::nullopt; 
        }
    }


    std::string_view FontTTF::GetFamily() const {
        return m_FontTTFFamily; 
    }


    // ?????????
    bool FontTTF::IsLoaded() const {
        return (m_face && m_face->family_name == m_FontTTFFamily);
    }

    
    FontTTF::~FontTTF() {
        FT_Done_Face(m_face);
        FT_Done_FreeType(m_lib);
    }


    bool FontTTF::operator==(const FontTTF& other) { 
        return m_FontTTFFamily == other.m_FontTTFFamily; 
    }

    bool FontTTF::operator!=(const FontTTF& other) { 
        return !(*this == other); 
    }


    void FontTTF::LoadGlyphs(void) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (char i = 0; i < 128; i++) {
            if (FT_Load_Char(m_face, i, FT_LOAD_RENDER)) {
                RUBY_ERROR("FreeType error: failed to load charecter {}({})", i, static_cast<GLuint>(i));
                continue;
            }

            TextureParams params;
            params.internalFormat = GL_RED;
            params.glFormat = GL_RED;

            Texture2D texture;
            texture.LoadByBuffer(m_face->glyph->bitmap.width,
                        m_face->glyph->bitmap.rows,
                        m_face->glyph->bitmap.buffer,
                        params);
        

            Glyph glyph;
            glyph.textureID = texture.GetTextureID();
            glyph.width = m_face->glyph->bitmap.width;
            glyph.height = m_face->glyph->bitmap.rows;
            glyph.bearingX = m_face->glyph->bitmap_left;
            glyph.bearingY = m_face->glyph->bitmap_top;
            glyph.advance = m_face->glyph->advance.x;

            m_chars[i] = glyph;
        }
    }


    bool FontTTF::FetchSystemFontTTF(const Path& name) const {
//        std::filesystem::path FontTTFsDest = (getPlatform() == PLATFORM_WINDOWS) ?
//                "C:\\Windows\\FontTTFs\\" : " /usr/local/share/FontTTFs";
//
//        auto&& completePath = FontTTFsDest / name;
//        if (FT_New_Face(m_lib, completePath.string().c_str(), 0, &m_face))
//            RUBY_ERROR("FontTTF::FetchSystemFontTTF() : failed to load specified FontTTF {}", name.string());
//
//        for (auto& FontTTF : std::filesystem::directory_iterator(FontTTFsDest))
//        {
//            auto&& FontTTFStr = FontTTF.path().string();
//
//            if (!FT_New_Face(m_lib, FontTTFStr.c_str(), 0, &m_face))
//                return true;
//        }
//
//        RUBY_CRITICAL("FontTTF::FetchSystemFontTTF() : failed to load any FontTTF");
        return false;
    }
}