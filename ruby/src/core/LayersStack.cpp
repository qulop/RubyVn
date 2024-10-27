#include "LayersStack.hpp"

#include <utility/Assert.hpp>


namespace Ruby {
    LayersStack::LayersStack()
        : m_bottomLayersBarrier(m_layers.end())
    {}

    size_t LayersStack::Size() const {
        return m_layers.size();
    }

    void LayersStack::PushBottomLayer(Layer* layer) {
        auto iter = std::find(begin(), m_bottomLayersBarrier, layer);
        if (iter != m_bottomLayersBarrier)  // if layer already exists
            return;

        layer->OnAttach();
        m_layers.emplace(m_bottomLayersBarrier, layer);
        m_bottomLayersBarrier++;
    }

    Layer* LayersStack::PopBottomLayer(Layer* layer) {
        if (!layer)
            return PopLastLayerFromStorage(m_bottomLayersBarrier);

        return PopLayerFromStorage(layer, begin(), m_bottomLayersBarrier);
    }

    void LayersStack::PushTopLayer(Layer* layer) {
        layer->OnAttach();
        m_layers.emplace_back(layer);

        if (m_bottomLayersBarrier == end())
            m_bottomLayersBarrier = std::prev(end());
    }

    Layer* LayersStack::PopTopLayer(Layer* layer) {
        if (!layer)
            return PopLastLayerFromStorage(end());

        return PopLayerFromStorage(layer, m_bottomLayersBarrier, end());
    }

    LayersStack::Iterator LayersStack::begin() {
        return m_layers.begin();
    }

    LayersStack::RevIterator LayersStack::rbegin() {
        return m_layers.rbegin();
    }

    LayersStack::Iterator LayersStack::end() {
      return m_layers.end();
    }

    LayersStack::RevIterator LayersStack::rend() {
        return m_layers.rend();
    }

    LayersStack::~LayersStack() {
        for (auto* layer : m_layers)
            delete layer;
    }



    Layer* LayersStack::PopLayerFromStorage(Layer* layer, Iterator begin, Iterator end) {
        RUBY_ASSERT_BASIC(layer != nullptr);

        auto iter = std::find(begin, end, layer);
        if (iter == end)
            return nullptr;

        layer->OnDetach();
        m_layers.erase(iter);

        return layer;
    }

    Layer* LayersStack::PopLastLayerFromStorage(Iterator barrier) {
        // if there are no layers or no bottom layers
        if (m_layers.empty() || barrier == begin())
            return nullptr;

        auto it = std::prev(barrier);
        auto tmp = *it;
        m_layers.erase(it);

        tmp->OnDetach();
        return tmp;
    }
}
