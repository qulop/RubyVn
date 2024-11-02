#include "Logger.hpp"

#include <platform/Platform.hpp>
#include <utility/Assert.hpp>
#include <utility/RubyUtility.hpp>


namespace Ruby {
    namespace Details::LoggerDetails {
        void destroyAppWithErrorBox(const RubyString& msg) {
            Platform::errorBox(msg, "Critical Error!");
            std::abort();
        }

        const char* logsDirectory = "logs";
        const char* defaultFileName = "log-from.log";
        const char* defaultLoggerName = "RubyEngine";
    }


    void Logger::Init(RubyPath loggerPath, const char* fileName, const char* coreName) {
        loggerPath /= Details::LoggerDetails::logsDirectory;
        if (!std::filesystem::exists(loggerPath))
            std::filesystem::create_directory(loggerPath);
        loggerPath /= fileName;

        auto& instLogger = GetInstance().m_logger;

        auto&& console = makeShared<Details::LoggerDetails::ConsoleSink>(spdlog::color_mode::always);
        console->set_pattern("<%m-%d-%Y %H:%M:%S> %^[%l]: %v%$");


        console->set_color(spdlog::level::debug, console->blue);
        console->set_color(spdlog::level::info, console->green);
        console->set_color(spdlog::level::warn, console->yellow);
        console->set_color(spdlog::level::err, console->red);
        console->set_color(spdlog::level::critical, console->magenta);

        // it will create new log file every 01:00 am
        auto&& daily = makeShared<Details::LoggerDetails::DailySink>(std::move(loggerPath.string()), 1, 0);
        daily->set_pattern("[%l] <%m-%d-%Y %H:%M:%S> - [thread: %t] [PID: %P]: %v");


        RubyVector<spdlog::sink_ptr> sinks = { std::move(console), std::move(daily) };
        instLogger = makeShared<Details::LoggerDetails::VendorLogger>(coreName, sinks.begin(), sinks.end());

        instLogger->set_level(RUBY_LOG_LEVEL);
        instLogger->flush_on(RUBY_LOG_LEVEL);
        spdlog::register_logger(instLogger);

        #undef LOG_LEVEL
    }


    SharedPtr<Details::LoggerDetails::VendorLogger> Logger::GetLogger() const {
        RUBY_ASSERT(m_logger != nullptr, "Logger cannot be empty: You must first call Logger::Init() before making a log!");

        return m_logger;
    }    

    bool Logger::IsInitialized() const {
        return m_logger != nullptr;
    }
}

