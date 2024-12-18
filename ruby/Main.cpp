#ifdef RUBY_RELEASE_BUILD
    #define _NDEBUG
#endif

#include "src/RubyEngine.hpp"
#ifdef RUBY_DEBUG_BUILD
    #include "tests/cases/TestCases.hpp"
    #include "tests/framework/RubyTestsFramework.hpp"
#endif

using namespace Ruby;

ProgramOptions getApplicationOptions(int argc, char** argv) {
    auto&& [defaultWidth, defaultHeight] = Platform::getScreenResolution();

    std::initializer_list<CmdLineOption> optionsList = {
        { "width", OptionArgType::INT, defaultWidth },
        { "height", OptionArgType::INT, defaultHeight },
        { "resizable", OptionArgType::BOOL, true },
        { "log-directory", OptionArgType::STRING },
        { "full-screen", OptionArgType::NONE },
        { "max-fps", OptionArgType::INT },
        { "test-mode", OptionArgType::NONE },
        { "merry-xmas", OptionArgType::NONE },  // For future fun purposes...
    };
    ProgramOptions opts { argc, argv, optionsList };

    return opts;
}


i32 main(int argc, char** argv) {
    auto options = std::move(getApplicationOptions(argc, argv));
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