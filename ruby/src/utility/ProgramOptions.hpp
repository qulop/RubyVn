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


    enum class OptionArgType {
        NONE,
        INT, BOOL,
        STRING
    };


    struct CmdLineOption {
        using ArgumentType = std::variant<std::monostate, i32, bool, String>;

        String longName;
        OptionArgType type = OptionArgType::STRING;
        ArgumentType defaultValue;


        CmdLineOption() = default;

        template<Details::ProgramOptions::AllowedArgumentType Tx>
        CmdLineOption(String longName, OptionArgType type, Tx defaultValue) :
            longName(std::move(longName)),
            type(type),
            defaultValue(std::move(defaultValue))
        {}

        CmdLineOption(String longName, OptionArgType type) :
            longName(std::move(longName)),
            type(type)
        {}
    };


    class RUBY_API ProgramOptions {
        using OptionsMapType = HashMap<String, CmdLineOption>;

    public:
        static bool IsFlag(std::string_view arg);

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
        RUBY_NODISCARD bool IsOptionExistsInTable(const OptionsMapType& map, const String& flag) const;
        RUBY_NODISCARD bool ParseArgument(const CmdLineOption& option, const char* argument);

        void AbortParse();

        void AddRemainingRequiredOptions(auto begin, auto end);

        OptionsMapType CreateTableOfMandatoryOptions(auto begin, auto end) const;


    private:
        i32 m_argc = 0;
        char** m_argv = nullptr;
        String m_appPath;

        std::atomic<bool> m_isParseProcessed = false;
        std::mutex m_parseMutex;

        HashMap<String, typename CmdLineOption::ArgumentType> m_options;
    };
}