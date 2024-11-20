#include "RendererAPI.hpp"

#include <glad/glad.h>


namespace Ruby::RendererAPI {
    void RUBY_API setViewport(u32 x, u32 y, u32 width, u32 height) {
        glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
                   static_cast<GLint>(width), static_cast<GLint>(height));
    }

    void RUBY_API clearBuffers() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RUBY_API clearColor(const Color& color) {
        glClearColor(color.Red(), color.Green(), color.Blue(), color.Alpha());
    }

    void RUBY_API clearColor(f32 r, f32 g, f32 b, f32 a) {
        glClearColor(r, g, b, a);
    }

    void RUBY_API drawIndexed(const VertexArray& vao, u32 indexCount) {
        if (!indexCount)
            indexCount = static_cast<u32>(vao.GetEBO().GetCount());

        vao.Bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
    }

    void RUBY_API drawLines(const VertexArray& vao, u32 vertexCount) {
        vao.Bind();
        glDrawArrays(GL_LINE, 0, static_cast<GLsizei>(vertexCount));
    }

    void RUBY_API setLineWidth(f32 width) {
        glLineWidth(width);
    }
}