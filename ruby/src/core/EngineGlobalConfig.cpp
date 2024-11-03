#include <core/EngineGlobalConfig.hpp>

namespace Ruby {
    namespace Globals {
        EngineGlobalConfig globalConfig;
    }


    String getParentPath(const String& path) {
        std::filesystem::path fsPath{ path };

        return fsPath.parent_path().string();
    }



    void EngineGlobalConfig::InitFromCommandLine(ProgramOptions&& opts) {
        if (opts.HasOption("max-fps"))
            maxFPS = std::any_cast<i32>(opts.GetArgumentOfOption("max-fps"));

        if (opts.HasOption("log-directory"))
            loggerBaseDirectory = std::any_cast<String>(opts.GetArgumentOfOption("log-directory"));
        else
            loggerBaseDirectory = std::move(getParentPath(opts.GetAppPath()));

        applicationDirectory = std::move(getParentPath(opts.GetAppPath()));

        videoConfig.width = std::any_cast<i32>(opts.GetArgumentOfOption("width"));
        videoConfig.height = std::any_cast<i32>(opts.GetArgumentOfOption("height"));
        videoConfig.isResizable = std::any_cast<bool>(opts.GetArgumentOfOption("resizable"));
        videoConfig.isFullScreened = opts.HasOption("full-screen");
    }

}
