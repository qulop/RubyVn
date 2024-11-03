#include "Layer.hpp"


namespace Ruby {
    static std::atomic<i64> s_unnamedLayersCounter = 0;


    Layer::Layer() {
        m_name = std::format("Layer {}", s_unnamedLayersCounter.load());

        s_unnamedLayersCounter.fetch_add(1);
    }

    Layer::Layer(const String& name) {
        m_name = name;
    }

    String Layer::GetName() const {
        return m_name;
    }
}