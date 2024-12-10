#include "RendererAPI.hpp"

#include <utility/RubyUtility.hpp>

#include <glad/glad.h>


namespace Ruby {
    void RendererAPI::Init(u32 width, u32 height) {
        if (!gladLoadGL())
            RUBY_CRITICAL("RendererAPI::Init() : Failed to load OpenGL via Glad!");

        SetViewport(0, 0, width, height);
    }

    String RendererAPI::GetVersionOfAPI() {
        return String{ (const char*)glGetString(GL_VERSION) };
    }

    String RendererAPI::GetVendor() {
        return String{ (const char*)glGetString(GL_VENDOR) };
    }

    String GetRendererName() {
        return String{ (const char*)glGetString(GL_RENDERER) };
    }

    GraphicAPIName RendererAPI::GetNameOfAPI() {
        return GraphicAPIName::OPENGL;
    }

    u32 RendererAPI::GetShadingLanguageVersion() {
        u32 ver = strToInt<u32>((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION))
            .value_or(0);
        
        return ver;
    }

    RendererAPI::ErrorType RendererAPI::GetError() {
        return glGetError();
    }


    i32 RendererAPI::GetMaxVertexAttribs() {
        i32 val = 0;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &val);

        return val;
    }



    void RendererAPI::SetViewport(u32 x, u32 y, u32 width, u32 height) {
        glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
                   static_cast<GLint>(width), static_cast<GLint>(height));
    }

    void RendererAPI::ClearBuffers() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererAPI::ClearColor(const Color& color) {
        glClearColor(color.Red(), color.Green(), color.Blue(), color.Alpha());
    }

    void RendererAPI::ClearColor(f32 r, f32 g, f32 b, f32 a) {
        glClearColor(r, g, b, a);
    }

    void RendererAPI::DrawElements(const VertexArray& vao, u32 indexCount) {
        if (!indexCount)
            indexCount = static_cast<u32>(vao.GetEBO().GetCount());

        vao.Bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
    }

    void RendererAPI::DrawLines(const VertexArray& vao, u32 vertexCount) {
        vao.Bind();
        glDrawArrays(GL_LINE, 0, static_cast<GLsizei>(vertexCount));
    }

    void RendererAPI::SetLineWidth(f32 width) {
        glLineWidth(width);
    }
}