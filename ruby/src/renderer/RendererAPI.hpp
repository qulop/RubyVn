#pragma once

#include <utility/Definitions.hpp>
#include <types/TypeTraits.hpp>
#include <graphics/Color.hpp>

#include "VertexArray.hpp"


namespace Ruby::RendererAPI {
    void RUBY_API setViewport(u32 x, u32 y, u32 width, u32 height);

    void RUBY_API clearBuffers();
    void RUBY_API clearColor(const Color& color);
    void RUBY_API clearColor(f32 r, f32 g, f32 b, f32 a);

    void RUBY_API drawIndexed(const VertexArray& vao, u32 indexCount=0);
    void RUBY_API drawLines(const VertexArray& vao, u32 vertexCount);
    void RUBY_API setLineWidth(f32 width);
}