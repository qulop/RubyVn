#include "EngineGlobalConfig.hpp"
#include "Application.hpp"
#include "Editor.hpp"

#include <renderer/Renderer.hpp>
#include <platform/Platform.hpp>
#include <types/Logger.hpp>
#include <utility/Assert.hpp>
#include <types/Timer.hpp>
#include <events/EventManager.hpp>


namespace Ruby {
    void Application::InitApplication(ProgramOptions&& opts) {
        g_globalConfig.InitFromCommandLine(std::move(opts));

        Logger::Init(g_globalConfig.loggerBaseDirectory);
        EventManager::Init();

        auto& a = EventManager::GetInstance();

        m_window = IWindow::Create(g_globalConfig.videoConfig);

        auto* editor = new Editor;
        PushTopLayer(editor);

        Renderer::Init();
        
        m_isInitialized.exchange(true, std::memory_order_relaxed);
    }

    
    bool Application::IsInitialized() const {
        return m_isInitialized.load(std::memory_order_relaxed);
    }


    void Application::StartApplication() {
        RUBY_ASSERT(m_isInitialized.load(std::memory_order_relaxed), 
            "Application must be initialized before launch!");

        auto lastTime = Time::getCurrentTimeRep();
        Time::TimeRep accumulator = 0;

        while (m_isRunning.load(std::memory_order_relaxed)) {
            auto& a = EventManager::GetInstance();

            auto currentTime = Time::getCurrentTimeRep();
            auto deltaTime = currentTime - lastTime;

            m_window->PollEvents();

            // Starts from top layers
            auto layerIt = m_layers.rbegin();
            for (layerIt; layerIt != m_layers.rend(); ++layerIt)
                (*layerIt)->Update();

            if (!m_window->Update())
                Finish();

            lastTime = currentTime;
        }
    }

    void Application::Finish() {
        m_isRunning.store(false, std::memory_order_seq_cst);
    }

    void Application::PushBottomLayer(Layer* layer) {
        m_layers.PushBottomLayer(layer);
    }

    void Application::PushTopLayer(Layer* layer) {
        m_layers.PushTopLayer(layer);
    }

    const Ptr<IWindow>& Application::GetWindow() const {
        return m_window;
    }
}