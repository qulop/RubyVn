#include "Logger.hpp"

#include <platform/Platform.hpp>
#include <utility/Assert.hpp>


namespace Ruby {
    namespace Details::LoggerDetails {
        void destroyApp(const RubyString& msg) {
            Platform::errorBox(msg, "Critical Error!");
            std::abort();
        }

        const char* logsDirectory = "logs";
        const char* defaultFileName = "log-from.log";
        const char* defaultLoggerName = "RubyEngine";
    }

    void Logger::Init(RubyPath loggerPath, const char* fileName, const char* coreName) {
        loggerPath.append(Details::LoggerDetails::logsDirectory)
                .append(fileName);

        auto& instLogger = GetInstance().m_logger;

        auto console = std::make_shared<Details::LoggerDetails::ConsoleSink>(spdlog::color_mode::always);
        console->set_pattern("<%m-%d-%Y %H:%M:%S> %^[%l]: %v%$");


        console->set_color(spdlog::level::debug, 1);        // Blue
        console->set_color(spdlog::level::info, 2);         // Green
        console->set_color(spdlog::level::warn, 6);         // Yellow
        console->set_color(spdlog::level::err, 4);          // Red
        console->set_color(spdlog::level::critical, 4);     // Red


        // it will create new log file every 01:00 am
        auto daily = std::make_shared<Details::LoggerDetails::DailySink>(loggerPath.string(), 1, 0);
        daily->set_pattern("[%l] <%m-%d-%Y %H:%M:%S> - [thread: %t] [PID: %P]: %v");


        std::vector<spdlog::sink_ptr> sinks = { std::move(console), std::move(daily) };

        instLogger = std::make_shared<Details::LoggerDetails::VendorLogger>(coreName, sinks.begin(), sinks.end());

        instLogger->set_level(LOG_LEVEL);
        instLogger->flush_on(LOG_LEVEL);
        spdlog::register_logger(instLogger);

        #undef LOG_LEVEL
    }


    Ptr<Details::LoggerDetails::VendorLogger> Logger::GetLogger() const {
        RUBY_ASSERT(m_logger != nullptr, "Logger cannot be empty: You must first call Logger::Init() before making log!");

        return m_logger;
    }    

    bool Logger::IsInitialized() const {
        return m_logger != nullptr;
    }
}

