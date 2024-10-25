#include "Shader.hpp"

#include <glad/glad.h>
#include <types/Logger.hpp>
#include <utility/Assert.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace Ruby {
    static void getShaderProcessError(GLuint target,
                                      ShaderTypes type = ShaderTypes::RUBY_SHADER_PROGRAM) {
        GLint hasNoErrors = 0;

        bool isShaderProgramPassed = (type == ShaderTypes::RUBY_SHADER_PROGRAM);
        if (isShaderProgramPassed)
            glGetProgramiv(target, GL_LINK_STATUS, &hasNoErrors);
        else
            glGetShaderiv(target, GL_COMPILE_STATUS, &hasNoErrors);

        if (hasNoErrors)
            return;

        GLchar buffer[256];
        auto bufferSize = static_cast<GLsizei>(std::size(buffer));
        if (isShaderProgramPassed)
            glGetProgramInfoLog(target, bufferSize, nullptr, buffer);
        else
            glGetShaderInfoLog(target, bufferSize, nullptr, buffer);

        auto typeName = EnumReflector::Create<ShaderTypes>().GetByValue(type).GetFieldName();
        auto linkingOrCompiling = (isShaderProgramPassed) ? "linking" : "compiling";

        RUBY_ERROR("{}: an error occurred while {} shader. {}", typeName, linkingOrCompiling, buffer);
    }


    template<size_t N, typename Type>
    void setUniformForType(u32 loc, const glm::vec<N, Type, glm::defaultp>& vec) {
        #ifdef RUBY_MSVC_USED
            #pragma warning(push)
            #pragma warning(disable : 4244)
        #endif
        static_assert((N >= 1 && N <= 4) && (std::is_floating_point_v<Type> || std::is_integral_v<Type>));

        constexpr bool isInt = std::is_integral_v<Type>;
        if constexpr (isInt) {
            if constexpr (N == 1)
                glUniform1i(loc, vec.x);
            else if constexpr (N == 2)
                glUniform2i(loc, vec.x, vec.y);
            else if constexpr (N == 3)
                glUniform3i(loc, vec.x, vec.y, vec.z);
            else
                glUniform4i(loc, vec.x, vec.y, vec.z, vec.w);
            return;
        }

        if constexpr (N == 1)
            glUniform1f(loc, vec.x);
        else if constexpr (N == 2)
            glUniform2f(loc, vec.x, vec.y);
        else if constexpr (N == 3)
            glUniform3f(loc, vec.x, vec.y, vec.z);
        else
            glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);

        #ifdef RUBY_MSVC_USED
            #pragma warning(pop)
        #endif
    }




    Shader::Shader(const RubyString& vertexPath, const RubyString& fragmentPath) {
        m_sources[RUBY_VERTEX_SHADER] = std::move(extractTextFromFile(vertexPath));
        m_sources[RUBY_FRAGMENT_SHADER] = std::move(extractTextFromFile(fragmentPath));
    }

    std::string_view Shader::GetSource(ShaderTypes type) const {
        return m_sources.at(type);
    }

    u32 Shader::GetShaderID(ShaderTypes type) const {
        return m_shadersId.at(type);
    }

    u32 Shader::GetProgramID() const {
        return m_programId;
    }

    u32 Shader::GetUniformLocation(const char* name) const {
        return glGetUniformLocation(m_programId, name);
    }

    void Shader::Bind() const {
        glUseProgram(m_programId);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    void Shader::AddSource(ShaderTypes type, const RubyString& src) {
        m_sources[type] = src;
    }

    void Shader::AddFile(ShaderTypes type, const RubyString& path) {
        m_sources[type] = std::move(extractTextFromFile(path));
    }

    bool Shader::IsEmpty() const {
        return m_sources.empty();
    }

    bool Shader::IsReady() const {
        return m_isReady;
    }

    void Shader::Compile() {
        for (const auto& [type, source]: m_sources)
            m_shadersId[type] = CompileShader(type, source.data());

        m_programId = glCreateProgram();
        for (const auto& id : m_shadersId)
            glAttachShader(m_programId, id.second);
        glLinkProgram(m_programId);

        getShaderProcessError(m_programId);

        RUBY_SWITCH_BOOL(m_isReady);
    }

    void Shader::SetFloat(const char* uniName, f32 value) const {
        glm::vec1 tmp{ value };
        setUniformForType<1, f32>(GetUniformLocation(uniName), tmp);
    }

    void Shader::SetFloat2(const char* uniName, const glm::vec2& vec) const {
        setUniformForType<2, f32>(GetUniformLocation(uniName), vec);
    }

    void Shader::SetFloat3(const char* uniName, const glm::vec3& vec) const {
        setUniformForType<3, f32>(GetUniformLocation(uniName), vec);
    }

    void Shader::SetFloat4(const char* uniName, const glm::vec4& vec) const {
        setUniformForType<4, f32>(GetUniformLocation(uniName), vec);
    }

    void Shader::SetFloatVector(const char* uniName, const f32* data, i32 count) const {
        auto loc = GetUniformLocation(uniName);
        glUniform1fv(loc, count, data);
    }

    void Shader::SetInt(const char* uniName, i32 value) const {
        glm::ivec1 tmp{ value };
        setUniformForType<1, i32>(GetUniformLocation(uniName), tmp);
    }

    void Shader::SetInt2(const char* uniName, const glm::ivec2& vec) const {
        setUniformForType<2, i32>(GetUniformLocation(uniName), vec);
    }

    void Shader::SetInt3(const char* uniName, const glm::ivec3& vec) const {
        setUniformForType<3, i32>(GetUniformLocation(uniName), vec);
    }

    void Shader::SetInt4(const char* uniName, const glm::ivec4& vec) const {
        setUniformForType<4, i32>(GetUniformLocation(uniName), vec);
    }

    void Shader::SetIntVector(const char* uniName, const i32* data, i32 count) const {
        auto loc = GetUniformLocation(uniName);
        glUniform1iv(loc, count, data);
    }

    void Shader::SetMat4(const char* uniName, const glm::mat4& mat) const {
        auto loc = GetUniformLocation(uniName);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    Shader::~Shader() {
        for (auto i : m_shadersId)
            glDeleteShader(i.second);
        glDeleteProgram(m_programId);
    }

    u32 Shader::CompileShader(ShaderTypes type, const char* source) {  // NOLINT
        if (type == ShaderTypes::RUBY_SHADER_PROGRAM)
            RUBY_WRECK("Shader::CompileShader() : this method can't compile shader program (RUBY_SHADER_PROGRAM received)");

        u32 id = glCreateShader(type);
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);

        getShaderProcessError(id, type);

        return id;
    }
}