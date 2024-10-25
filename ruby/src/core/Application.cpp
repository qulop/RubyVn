#include <renderer/Renderer.hpp>
#include <platform/Platform.hpp>
#include <types/Logger.hpp>
#include <utility/Assert.hpp>

#include "EngineGlobalConfig.hpp"
#include "Application.hpp"
#include <types/Timer.hpp>

#include "Editor.hpp"


namespace Ruby {
    void Application::InitApplication(ProgramOptions&& opts) {
        g_globalConfig.InitFromCommandLine(std::move(opts));

        Logger::GetInstance().InitLogger(g_globalConfig.loggerBaseDirectory);

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
            auto currentTime = Time::getCurrentTimeRep();
            auto deltaTime = currentTime - lastTime;

            m_window->PollEvents();

            // Starts from top layers
            auto layersIt = std::rbegin(m_layers);
            for (layersIt; layersIt != std::rend(m_layers); layersIt++) {
                auto layer = *layersIt;
                layer->Update();
            }

            if (!m_window->Update())
                Finish();

            lastTime = currentTime;
        }
    }

    void Application::Finish() {
        m_isRunning.store(false);
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