#pragma once

#ifdef RUBY_MSVC_USED
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ansicolor_sink.h>

#include <utility/Definitions.hpp>
#include <types/Singleton.hpp>

#include "StdInc.hpp"


namespace Ruby {
    class RUBY_API Logger;  // Declaration for critical function (see bellow)


    namespace Details::LoggerDetails {
        using VendorLogger      = spdlog::logger;
        using DailySink         = spdlog::sinks::daily_file_sink_mt;
        using ConsoleSink       = spdlog::sinks::ansicolor_stdout_sink_mt;

        extern const char* logsDirectory;
        extern const char* defaultFileName;
        extern const char* defaultLoggerName;

        void destroyAppWithErrorBox(const String& msg);

        template<typename... Args>
        void critical(spdlog::format_string_t<Args...> format, Args&&... args) {
            auto&& msg = fmt::format(format, std::forward<Args>(args)...);
            Logger::GetInstance().GetLogger()->critical(std::move(format), std::forward<Args>(args)...);

            destroyAppWithErrorBox(msg);
        }

        template<typename Tx>
        void critical(const Tx& format) {
            Logger::GetInstance().GetLogger()->critical(format);

            destroyAppWithErrorBox(format);
        }
    }


    class RUBY_API Logger final : public Singleton<Logger> {
        RUBY_DEFINE_SINGLETON(Logger);

    public:
        using LoggerPtr = SharedPtr<Details::LoggerDetails::VendorLogger>;

    public:
        static void Init(Path loggerPath,
                       const char* fileName = Details::LoggerDetails::defaultFileName,
                       const char* coreName = Details::LoggerDetails::defaultLoggerName);

    public:
        RUBY_NODISCARD LoggerPtr GetLogger() const;
        RUBY_NODISCARD bool IsInitialized() const;

    private:
        LoggerPtr m_logger = nullptr;
    };
}

#define RUBY_DEBUG(...)            Ruby::Logger::GetInstance().GetLogger()->debug(__VA_ARGS__)
#define RUBY_INFO(...)             Ruby::Logger::GetInstance().GetLogger()->info(__VA_ARGS__)
#define RUBY_WARNING(...)          Ruby::Logger::GetInstance().GetLogger()->warn(__VA_ARGS__)
#define RUBY_ERROR(...)            Ruby::Logger::GetInstance().GetLogger()->error(__VA_ARGS__)
#define RUBY_CRITICAL(...)         Ruby::Details::LoggerDetails::critical(__VA_ARGS__)

#ifdef RUBY_MSVC_USED
    #pragma warning(pop)
#endif
