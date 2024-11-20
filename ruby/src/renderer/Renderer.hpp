#pragma once

#include <graphics/Color.hpp>
#include "VertexArray.hpp"
#include "Buffers.hpp"


namespace Ruby {
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