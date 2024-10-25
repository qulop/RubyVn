#pragma once

#include <utility/Definitions.hpp>
#include <events/IEvent.hpp>


namespace Ruby {
    abstract class RUBY_API Layer {
    public:
        Layer();
        Layer(const RubyString& name);  // NOLINT

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnEvent(IEvent* event) = 0;
        virtual void Update() = 0;

        RUBY_NODISCARD RubyString GetName() const;

        virtual ~Layer() = default;

    private:
        RubyString m_name;
    };
}