#pragma once

#include <utility/Definitions.hpp>
#include <events/IEvent.hpp>


namespace Ruby {
    abstract class RUBY_API Layer {
    public:
        Layer();
        Layer(const String& name);  // NOLINT

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnEvent(IEvent* event) = 0;
        virtual void Update() = 0;

        RUBY_NODISCARD String GetName() const;

        virtual ~Layer() = default;

    private:
        String m_name;
    };
}