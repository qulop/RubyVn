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

        RubyString applicationDirectory; // Path to the directory where the engine executable is located
        RubyString loggerBaseDirectory; // Location(path) of logger's directory 
        
        VideoStruct videoConfig;

        Ptr<wchar_t> engineLocale;
        Ptr<char> engineEncoding;

    private:
        std::mutex m_mutex;

    public:
        void InitFromCommandLine(ProgramOptions&& opts);
    };

    
    extern EngineGlobalConfig g_globalConfig;
}