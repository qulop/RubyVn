#pragma once

#include <types/StdInc.hpp>

#include "Enum.hpp"
#include "Definitions.hpp"


namespace Ruby {
    namespace Details::ProgramOptions {
        template<typename Tx>
        concept AllowedArgumentType = 
            std::integral<Tx> || 
            std::same_as<Tx, String> ||
            std::same_as<Tx, const char*>;
    }


    RUBY_ENUM(OptionArgumentType,
        CLI_ARG_NONE,
        CLI_ARG_INT, CLI_ARG_BOOL,
        CLI_ARG_STRING
    );


    struct CmdLineOption {
        using ArgumentType = std::variant<std::monostate, i32, bool, String>;

        String longName;
        OptionArgumentType type = CLI_ARG_STRING;
        ArgumentType defaultValue;


        CmdLineOption() = default;

        template<Details::ProgramOptions::AllowedArgumentType Tx>
        CmdLineOption(String longName, OptionArgumentType type, Tx defaultValue) :
            longName(std::move(longName)),
            type(type),
            defaultValue(std::move(defaultValue))
        {}

        CmdLineOption(String longName, OptionArgumentType type) :
            longName(std::move(longName)),
            type(type)
        {}
    };


    class RUBY_API ProgramOptions {
        using OptionsMapType = HashMap<String, CmdLineOption>;
        using InitalizerListConstIterator = typename std::initializer_list<CmdLineOption>::const_iterator;

    public:
        static bool IsFlag(const char* arg);

    public:
        ProgramOptions() = default;
        ProgramOptions(i32 argc, char** argv, std::initializer_list<CmdLineOption> opts);

        ProgramOptions(const ProgramOptions& other);
        ProgramOptions(ProgramOptions&& other) noexcept;

        RUBY_NODISCARD bool IsParseProcessed() const;

        char* At(size_t i);
        char* operator[](size_t i);

        RUBY_NODISCARD bool HasOption(const String& opt) const;
        RUBY_NODISCARD std::any GetArgumentOfOption(const String& opt) const;

        RUBY_NODISCARD i32 GetCount() const;
        char** GetRawOptions();
        RUBY_NODISCARD String GetAppPath() const;

        ProgramOptions& operator=(const ProgramOptions& other);
        ProgramOptions& operator=(ProgramOptions&& other) noexcept;

        ~ProgramOptions();
        
    private:
        void CopyRawOptions(char** argv);

        RUBY_NODISCARD bool ExtractOptionName(String& arg) const;

        void AddRemainingRequiredOptions(InitalizerListConstIterator begin, InitalizerListConstIterator end);

        RUBY_NODISCARD bool IsOptionExistsInTable(const OptionsMapType& map, const String& flag) const;
        OptionsMapType CreateTableOfMandatoryOptions(InitalizerListConstIterator begin, InitalizerListConstIterator end) const;
        bool ParseArgumentForOption(const CmdLineOption& opt, const char* arg);


    private:
        int m_argc = 0;
        char** m_argv = nullptr;
        String m_appPath;

        std::atomic<bool> m_isParseProcessed = false;
        std::mutex m_parseMutex;

        HashMap<String, typename CmdLineOption::ArgumentType> m_options;
    };
}