#pragma once

#include <utility/Definitions.hpp>
#include <types/TypeTraits.hpp>
#include <graphics/Color.hpp>

#include "VertexArray.hpp"


namespace Ruby {
    enum class GraphicAPIName {
        NONE,
        OPENGL, VULKAN, DIRECTX
    };


    class RendererAPI {
    public:
        using ErrorType = GLenum;

        static void Init(u32 width, u32 height);

        static String GetVersionOfAPI();
        static String GetVendor();
        static String GetRendererName();
        static GraphicAPIName GetNameOfAPI();
        static u32 GetShadingLanguageVersion();
        static ErrorType GetError();

        static i32 GetMaxVertexAttribs();

        static void SetViewport(u32 x, u32 y, u32 width, u32 height);

        static void ClearBuffers();
        static void ClearColor(const Color& color);
        static void ClearColor(f32 r, f32 g, f32 b, f32 a);

        static void DrawElements(const VertexArray& vao, u32 indexCount=0);
        static void DrawLines(const VertexArray& vao, u32 vertexCount);
        static void SetLineWidth(f32 width);
    };
}