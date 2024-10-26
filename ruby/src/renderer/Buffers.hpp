#pragma once

#include <utility/Definitions.hpp>
#include "ShaderDataTypes.hpp"


namespace Ruby {
    namespace Details::Renderer {
        struct _VBOLayoutElement {
            _VBOLayoutElement() = default;
            explicit _VBOLayoutElement(ShaderDataTypes type, bool isNormalized = false) :
                type(type),
                count(getShaderTypeElementsCount(type)),
                countPerLine(getShaderTypeElementsCount(type, false)),
                size(getShaderDataTypeSize(type)),
                isNormalized(isNormalized)
            {}

            i32 count = 0;
            i32 countPerLine = 0;
            i32 size = 0;
            ShaderDataTypes type = ShaderDataTypes::NONE;
            bool isNormalized = false;
            size_t offset = 0;
        };


        class _VBOLayout {
        public:
            using ElementType = _VBOLayoutElement;

            _VBOLayout() = default;

            void Set(std::initializer_list<ShaderDataTypes> types);

            RUBY_NODISCARD i32 GetStride() const {
                return m_stride;
            }

            RUBY_NODISCARD const RubyVector<ElementType>& GetElements() const {
                return m_elements;
            }

        private:
            void CalculateStrideAndOffset();

        private:
            RubyVector<ElementType> m_elements;
            i32 m_stride = 0;
        };
    }


    class RUBY_API VertexBuffer {
    public:
        using LayoutType = Details::Renderer::_VBOLayout;

        VertexBuffer() = default;
        explicit VertexBuffer(size_t size);
        VertexBuffer(f32* vertices, size_t size);

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, size_t size);
        void SetLayoutTypes(std::initializer_list<ShaderDataTypes> layout);
        RUBY_NODISCARD const LayoutType& GetLayout() const;

        ~VertexBuffer();

    private:
        u32 m_id = RUBY_UNDEFINED_ID;
        LayoutType m_layout;
    };


    class RUBY_API IndexBuffer {
    public:
        IndexBuffer() = default;
        IndexBuffer(f32* indices, size_t size);

        void Bind() const;
        void Unbind() const;

        void SetData(f32* indices, size_t size);

        RUBY_NODISCARD size_t GetCount() const;

        ~IndexBuffer();

    private:
        u32 m_id = RUBY_UNDEFINED_ID;
        size_t m_count = 0;
    };
}