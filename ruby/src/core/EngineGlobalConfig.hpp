#pragma once

#include "WindowProps.hpp"

#include <utility/ProgramOptions.hpp>
#include <utility/Definitions.hpp>
#include <types/Logger.hpp>


namespace Ruby {
    struct EngineGlobalConfig {
    public:
        u16 maxFPS = 120;
        const f32 timestep = 1000.0f / static_cast<f32>(maxFPS);
        const u16 interval = 1; // time step after how long fps will be calculated

        String applicationDirectory; // Path to the directory where the engine executable is located
        String loggerBaseDirectory; // Location(path) of logger's directory 
        
        VideoStruct videoConfig;

        SharedPtr<wchar_t> engineLocale;
        SharedPtr<char> engineEncoding;

    private:
        std::mutex m_mutex;

    public:
        void InitFromCommandLine(ProgramOptions&& opts);
    };

    namespace Globals {
        extern EngineGlobalConfig globalConfig;
    }
}