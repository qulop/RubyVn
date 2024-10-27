#pragma once

#include "Layer.hpp"
#include <types/StdInc.hpp>


namespace Ruby {
    class RUBY_API LayersStack {
        using StorageType = std::list<Layer*>;
    public:
        using Iterator = StorageType::iterator;
        using RevIterator = StorageType::reverse_iterator;

        LayersStack();

        RUBY_NODISCARD size_t Size() const;

        void PushBottomLayer(Layer* layer);
        Layer* PopBottomLayer(Layer* layer=nullptr);

        void PushTopLayer(Layer* layer);
        Layer* PopTopLayer(Layer* layer=nullptr);

        RUBY_NODISCARD Iterator begin();
        RUBY_NODISCARD RevIterator rbegin();

        RUBY_NODISCARD Iterator end();
        RUBY_NODISCARD RevIterator rend();

        ~LayersStack();

    private:
        Layer* PopLayerFromStorage(Layer* layer, Iterator begin, Iterator end);
        Layer* PopLastLayerFromStorage(Iterator barrier);

    private:
        StorageType m_layers;
        Iterator m_bottomLayersBarrier;  // points to the first top layer
    };
}