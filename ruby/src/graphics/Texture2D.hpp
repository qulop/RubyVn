#pragma once

#include <utility/RubyUtility.hpp>

#include <stb_image.h>
#include <glad/glad.h>


namespace Ruby {
    struct TextureParams {
        enum TextureWrap {
            CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
            CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
            REPEAT = GL_REPEAT,
            MIRRORED_REPEAT = GL_MIRRORED_REPEAT
        };

        enum TextureFilter {
            LINEAR              = GL_LINEAR,
            NEAREST             = GL_NEAREST
        };

        enum ImageFormat {
            DEFAULT = STBI_default,
            GREY = STBI_grey,
            GREY_ALPHA = STBI_grey_alpha,
            RGB = STBI_rgb,
            RGBA = STBI_rgb_alpha
        };


        TextureFilter filter = LINEAR;
        TextureWrap wrap = CLAMP_TO_EDGE;
        ImageFormat imageFormat = RGB;

        GLint internalFormat = GL_RGB;
        GLenum glFormat = GL_RGB;
    };


    class Texture2D {
    public:
        using Deleter = std::function<void(u8*)>;

        Texture2D() = default;
        explicit Texture2D(const String& path, TextureParams params={});

        void LoadByPath(const String& path, TextureParams params={});
        void LoadByBuffer(i32 width, i32 height, u8* buffer, TextureParams params={});

        void AddDeleter(const Deleter& deleter);

        void Bind() const;
        void Unbind() const;

        RUBY_NODISCARD const u8* GetData() const;
        RUBY_NODISCARD i32 GetWidth() const;
        RUBY_NODISCARD i32 GetHeight() const;

        RUBY_NODISCARD GLuint GetTextureID() const;

        ~Texture2D();

    private:
        u8* m_data = nullptr;
        i32 m_width = 0;
        i32 m_height = 0;

        Deleter m_deleter = [](u8* data) { stbi_image_free(data); };
        GLuint m_texture = -1;
    };
}