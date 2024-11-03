#pragma once

#include <utility/RubyUtility.hpp>
#include <types/StdInc.hpp>

#include <glm/glm.hpp>


namespace Ruby {
    RUBY_ENUM(ShaderTypes,
        RUBY_SHADER_PROGRAM,
        RUBY_FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        RUBY_VERTEX_SHADER = GL_VERTEX_SHADER
    );


    class RUBY_API Shader {
    public:
        Shader() = default;
        Shader(const String& vertexPath, const String& fragmentPath);

        RUBY_NODISCARD std::string_view GetSource(ShaderTypes type) const;
        RUBY_NODISCARD u32 GetShaderID(ShaderTypes type) const;
        RUBY_NODISCARD u32 GetProgramID() const;
        RUBY_NODISCARD u32 GetUniformLocation(const char* name) const;

        void Bind() const;
        void Unbind() const;

        void AddSource(ShaderTypes type, const String& src);
        void AddFile(ShaderTypes type, const String& path);

        RUBY_NODISCARD bool IsEmpty() const;
        RUBY_NODISCARD bool IsReady() const;

        void Compile();

        void SetFloat(const char* uniName, f32 value) const;
        void SetFloat2(const char* uniName, const glm::vec2& vec) const;
        void SetFloat3(const char* uniName, const glm::vec3& vec) const;
        void SetFloat4(const char* uniName, const glm::vec4& vec) const;
        void SetFloatVector(const char* uniName, const f32* data, i32 count) const;

        void SetInt(const char* uniName, i32 value) const;
        void SetInt2(const char* uniName, const glm::ivec2& vec) const;
        void SetInt3(const char* uniName, const glm::ivec3& vec) const;
        void SetInt4(const char* uniName, const glm::ivec4& vec) const;
        void SetIntVector(const char* uniName, const i32* data, i32 count) const;

        void SetMat4(const char* uniName, const glm::mat4& mat) const;

        ~Shader();

    private:
        u32 CompileShader(ShaderTypes type, const char* source);

    private:
        HashMap<ShaderTypes, u32> m_shadersId;
        HashMap<ShaderTypes, String> m_sources;

        u32 m_programId = RUBY_UNDEFINED_ID;
        std::atomic<bool> m_isReady = false;
    };
}