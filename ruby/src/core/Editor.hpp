#pragma once

#include "Application.hpp"
#include "Layer.hpp"
#include "IWindow.hpp"


namespace Ruby {
    class Editor : public Layer {
    public:
        Editor();

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(IEvent* event) override;
        void Update() override;

        ~Editor() override = default;
    };
}