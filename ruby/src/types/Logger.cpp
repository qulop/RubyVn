#include "Logger.hpp"

#include <platform/Platform.hpp>
#include <utility/Assert.hpp>


namespace Ruby {
    namespace Details::RubyLogger {
        void destroyApp(const RubyString& msg) {
            Platform::errorBox(msg, "Critical Error!");
            std::abort();
        }

        const char* logsDirectory = "logs";
        const char* defaultFile = "log-from.log";
        const char* defaultLoggerName = "RubyCore";
    }


    Logger::Ptr<Details::RubyLogger::VendorLogger> Logger::GetVendorLogger() const {
        RUBY_ASSERT(m_logger != nullptr, "Logger cannot be empty: You must first call Logger::Init() before making log!");

        return m_logger;
    }    

    bool Logger::IsInitialized() const {
        return m_logger != nullptr;
    }


    void Logger::InitLogger(std::filesystem::path loggerPath, const char* fileName, const char* coreName) {
        loggerPath
            .append(Details::RubyLogger::logsDirectory)
            .append(fileName);

        auto console = std::make_shared<Details::RubyLogger::ConsoleSink>(spdlog::color_mode::always);
        console->set_pattern("<%m-%d-%Y %H:%M:%S> %^[%l]: %v%$");

        
        console->set_color(spdlog::level::debug, 1);        // Blue
        console->set_color(spdlog::level::info, 2);         // Green
        console->set_color(spdlog::level::warn, 6);         // Yellow
        console->set_color(spdlog::level::err, 4);          // Red
        console->set_color(spdlog::level::critical, 4);     // Red


        // it will create new log file every 01:00 am
        auto daily = std::make_shared<Details::RubyLogger::DailySink>(loggerPath.string(), 1, 0);
        daily->set_pattern("[%l] <%m-%d-%Y %H:%M:%S> - [thread: %t] [PID: %P]: %v");


        std::vector<spdlog::sink_ptr> sinks = { std::move(console), std::move(daily) };

        m_logger = std::make_shared<Details::RubyLogger::VendorLogger>(coreName, sinks.begin(), sinks.end());

        m_logger->set_level(LOG_LEVEL);
        m_logger->flush_on(LOG_LEVEL);
        spdlog::register_logger(m_logger);

        #undef LOG_LEVEL
    }
}

