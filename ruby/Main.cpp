#ifdef RUBY_RELEASE_BUILD
    #define _NDEBUG
#endif

#include "src/RubyEngine.hpp"
#ifdef RUBY_DEBUG_BUILD
    #include "tests/cases/TestCases.hpp"
    #include "tests/framework/RubyTestsFramework.hpp"
#endif

using namespace Ruby;


ProgramOptions initApplicationOptions(int argc, char** argv) {
    using Ruby::OptionArgumentType;

    auto&& [screenX, screenY] = Platform::getScreenResolution();
    auto&& [defaultWidth, defaultHeight] = std::make_pair(static_cast<i32>(screenX * 0.9),
                                                          static_cast<i32>(screenY * 0.95));

    ProgramOptions opts { argc, argv, {
        { "width", CLI_ARG_INT, defaultWidth },
        { "height", CLI_ARG_INT, defaultHeight },
        { "resizable", CLI_ARG_BOOL, true },
        { "log-directory", CLI_ARG_STRING },
        { "full-screen", CLI_ARG_NONE },
        { "max-fps", CLI_ARG_INT },
        { "test-mode", CLI_ARG_NONE },
    }};

    return opts;
}


i32 main(int argc, char** argv) {
    auto options = std::move(initApplicationOptions(argc, argv));
    if (!options.IsParseProcessed())
        return EXIT_FAILURE;

    #ifdef RUBY_DEBUG_BUILD
        if (options.HasOption("test-mode")) // NOLINT
            return RUBY_RUN_ALL_TESTS(options);
    #endif

    auto& app = Application::GetInstance();
    app.InitApplication(std::move(options));

    if (!app.IsInitialized()) {
        Platform::writeInConsole("Failed to initialize an application (╥﹏╥)");
        return EXIT_FAILURE;
    }

    app.StartApplication();
    return EXIT_SUCCESS;
}