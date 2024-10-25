#pragma once

#include <utility/Definitions.hpp>
#include <utility/ProgramOptions.hpp>
#include <types/Singleton.hpp>

#include "LayersStack.hpp"
#include "IWindow.hpp"


namespace Ruby {
    class RUBY_API Application : public Singleton<Application> {
        RUBY_DEFINE_SINGLETON(Application)

    public:
        void InitApplication(ProgramOptions&& opts);

        RUBY_NODISCARD bool IsInitialized() const;

        void StartApplication();
        void Finish();

        void PushBottomLayer(Layer* layer);
        void PushTopLayer(Layer* layer);

        RUBY_NODISCARD const Ptr<IWindow>& GetWindow() const;

        ~Application() override = default;

    private:
        LayersStack m_layers;
        Ptr<IWindow> m_window;  // I still think that std::unique_ptr needed here

        std::atomic<bool> m_isInitialized = false;
        std::atomic<bool> m_isRunning = true;
    };
}