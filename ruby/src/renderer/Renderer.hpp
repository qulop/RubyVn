#pragma once

#include <graphics/Color.hpp>
#include "VertexArray.hpp"
#include "Buffers.hpp"


namespace Ruby {
    void RUBY_API initRendererAPI();

    void RUBY_API setViewport(u32 x, u32 y, u32 width, u32 height);

    void RUBY_API clearBuffers();
    void RUBY_API clearColor(const Color& color);
    void RUBY_API clearColor(f32 r, f32 g, f32 b, f32 a);

    void RUBY_API drawIndexed(const VertexArray& vao, u32 indexCount);
    void RUBY_API drawLines(const VertexArray& vao, u32 vertexCount);
    void RUBY_API setLineWidth(f32 width);


    class RUBY_API Renderer {
    public:
        static void Init();
        static void Shutdown();

        static void DrawRotatedObject();

        static void DrawQuad();


        static void DrawLine();

        static void DrawCircle();

        static void DrawChar();
        static void DrawString();
    };
}