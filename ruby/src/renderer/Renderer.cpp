#include "Renderer.hpp"

#include <utility/Definitions.hpp>
#include <types/StdInc.hpp>
#include <glad/glad.h>
#include <utility/Assert.hpp>


namespace Ruby {
    void initRendererAPI() {

    }

    void setViewport(u32 x, u32 y, u32 width, u32 height) {
        glViewport(x, y, width, height);
    }

    void clearBuffers() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void clearColor(const Color& color) {
        glClearColor(color.Red(), color.Green(), color.Blue(), color.Alpha());
    }

    void clearColor(f32 r, f32 g, f32 b, f32 a) {
        glClearColor(r, g, b, a);
    }

    void drawIndexed(const VertexArray& vao, u32 indexCount) {
        if (!indexCount)
            indexCount = static_cast<u32>(vao.GetEBO().GetCount());

        vao.Bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void drawLines(const VertexArray& vao, u32 vertexCount){
        vao.Bind();
        glDrawArrays(GL_LINE, 0, vertexCount);
    }

    void setLineWidth(f32 width) {
        glLineWidth(width);
    }


    void Renderer::Init() {
        initRendererAPI();

    }


    void Renderer::Shutdown() {
        RUBY_NOT_IMPLEMENTED();
    }

    void Renderer::DrawRotatedObject() { 
        RUBY_NOT_IMPLEMENTED(); 
    }


    void Renderer::DrawQuad() { 
        RUBY_NOT_IMPLEMENTED(); 
    }


    void Renderer::DrawLine() { 
        RUBY_NOT_IMPLEMENTED(); 
    }


    void Renderer::DrawCircle() { 
        RUBY_NOT_IMPLEMENTED(); 
    }


    void Renderer::DrawChar() { 
        RUBY_NOT_IMPLEMENTED(); 
    }


    void Renderer::DrawString() { 
        RUBY_NOT_IMPLEMENTED(); 
    }
}