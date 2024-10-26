#include "ProgramOptions.hpp"
#include "RubyUtility.hpp"

#include <utility/Assert.hpp>
#include <platform/Platform.hpp>
#include <types/Logger.hpp>


namespace Ruby {
    OptionArgumentType deduceTypeOfArgument(const char* arg) {
        if (std::strcmp(arg, "true") == 0 || std::strcmp(arg, "false") == 0)
            return CLI_ARG_BOOL;
        else if (strToInt<i32>(arg).has_value())
            return CLI_ARG_INT;
        return CLI_ARG_STRING;
    }



    ProgramOptions::ProgramOptions(i32 argc, char** argv, std::initializer_list<CmdLineOption> opts) :
            m_argc(argc),
            m_appPath(argv[0])
    {
        std::lock_guard<std::mutex> guard{ m_parseMutex };

        CopyRawOptions(argv);
        RUBY_ASSERT_BASIC(m_argv != nullptr);

        bool hasError = false;
        OptionsMapType optionsMap = std::move(CreateTableOfMandatoryOptions(opts.begin(), opts.end()));
        
        for (auto tokenIndex = 0; tokenIndex < (m_argc - 1); tokenIndex++) {
            RubyString token = At(tokenIndex);

            if (!IsFlag(token.c_str())) {
                writeInConsoleF("Argument doesn't apply to any flag: \"{}\"\n", token);
                continue;
            }

            bool hasErrorOnIteration = (!ExtractOptionName(token)
                    || !IsOptionExistsInTable(optionsMap, token)
                    || !ParseArgumentForOption(optionsMap.at(token), At(tokenIndex + 1)));

            if (hasErrorOnIteration) {
                hasError = true;
                continue;
            }
            tokenIndex += (optionsMap.at(token).type == CLI_ARG_NONE) ? 0 : 1;
        }

        if (hasError)
            return;

        AddRemainingRequiredOptions(opts.begin(), opts.end());
        m_isParseProcessed.exchange(true);
    }

    ProgramOptions::ProgramOptions(const ProgramOptions& other) {
        *this = other;
    }

    ProgramOptions::ProgramOptions(ProgramOptions&& other) noexcept {
        *this = std::move(other);
    }

    bool ProgramOptions::IsParseProcessed() const {
        return m_isParseProcessed.load();
    }

    bool ProgramOptions::IsFlag(const char* arg) {
        return (arg != nullptr && arg[0] == '-');
    }

    char* ProgramOptions::At(size_t i) {
        return operator[](i);
    }

    char* ProgramOptions::operator[](size_t i) {
        RUBY_ASSERT(i < m_argc, "Index out of borders");

        return m_argv[i];
    }

    bool ProgramOptions::HasOption(const RubyString& opt) const {
        return m_options.contains(opt);
    }

    std::any ProgramOptions::GetArgumentOfOption(const RubyString& opt) const {
        if (!HasOption(opt) || std::holds_alternative<std::monostate>(m_options.at(opt)))
            return std::any{};

        auto variantGetter = [](const auto& val) -> std::any {
            return std::make_any<std::decay_t<decltype(val)>>(val);
        };

        return std::visit(variantGetter, m_options.at(opt));
    }

    i32 ProgramOptions::GetCount() const {
        return m_argc;
    }

    char** ProgramOptions::GetRawOptions() {
        return m_argv;
    }

    RubyString ProgramOptions::GetAppPath() const {
        return m_appPath;
    }


    ProgramOptions& ProgramOptions::operator=(const ProgramOptions& other) {
        if (this == &other)
            return *this;

        m_argc = other.m_argc;
        m_appPath = other.m_appPath;
        CopyRawOptions(other.m_argv);
        m_options = other.m_options;
        m_isParseProcessed.exchange(other.m_isParseProcessed.load());

        return *this;
    }

    ProgramOptions& ProgramOptions::operator=(ProgramOptions&& other) noexcept {
        if (this == &other)
            return *this;

        m_argc = std::exchange(other.m_argc, 0);
        m_argv = std::exchange(other.m_argv, nullptr);
        m_appPath = std::move(other.m_appPath);
        m_options = std::move(other.m_options);
        m_isParseProcessed.store(other.m_isParseProcessed.exchange(false));

        return *this;
    }

    ProgramOptions::~ProgramOptions() {
        delete[] m_argv;
    }



    void ProgramOptions::CopyRawOptions(char** args) {
        m_argv = new char*[m_argc];  // content of m_argv(without path) + nullptr limiter

        for (auto i = 1; i < m_argc; i++) {
            size_t len = std::strlen(args[i]) + 1;
            m_argv[i-1] = new char[len];
            strcpy_s(m_argv[i-1], len, args[i]);
        }

        m_argv[m_argc - 1] = nullptr;
    }

    bool ProgramOptions::ExtractOptionName(RubyString& arg) const {
        size_t beginOfFlagName = arg.find_first_not_of('-');
        if (beginOfFlagName == std::string::npos) {
            writeInConsoleF("Failed to find name of option: \"--{}\"\n", arg);
            return false;
        }

        arg.erase(0, beginOfFlagName);
        return true;
    }

    void ProgramOptions::AddRemainingRequiredOptions(InitalizerListConstIterator begin, InitalizerListConstIterator end) {
        for (auto opt = begin; opt != end; opt++) {
            bool isDefaultValueExists = !std::holds_alternative<std::monostate>(opt->defaultValue);

            if (m_options.contains(opt->longName) || !isDefaultValueExists)
                continue;
            m_options[opt->longName] = opt->defaultValue;
        }
    }

    bool ProgramOptions::IsOptionExistsInTable(const ProgramOptions::OptionsMapType& map, const RubyString& flag) const {
        if (map.contains(flag))
            return true;

        writeInConsoleF("Unknown option is found - \"--{}\"\n", flag);
        return false;
    }

    ProgramOptions::OptionsMapType ProgramOptions::CreateTableOfMandatoryOptions(InitalizerListConstIterator begin, InitalizerListConstIterator end) const {
        OptionsMapType ret;
        for (auto opt = begin; opt != end; opt++)
            ret[opt->longName] = *opt;

        return ret;
    }

    bool ProgramOptions::ParseArgumentForOption(const CmdLineOption& opt, const char* arg) {
        if (opt.type == CLI_ARG_NONE) {
            m_options[opt.longName] = std::monostate{};
            return true;
        }

        if (!arg || IsFlag(arg)) {
            writeInConsoleF("Missing argument for option \"--{}\"\n",
               opt.longName);
            return false;
        }

        if (auto argType = deduceTypeOfArgument(arg); argType != opt.type) {
            auto&& reflector = EnumReflector::Create<OptionArgumentType>();

            writeInConsoleF("Option \"--{}\" expects argument of type {}, but gets {}\n",
               opt.longName,
               reflector.GetByValue(opt.type).GetFieldName(),
               reflector.GetByValue(argType).GetFieldName());
            return false;
        }

        switch (opt.type) {
            case CLI_ARG_INT:
                m_options[opt.longName] = strToInt<i32>(arg).value(); break;
            case CLI_ARG_BOOL:
                m_options[opt.longName] = strToBool(arg).value(); break;
            default:    // Always assumes a string
                m_options[opt.longName] = RubyString{ arg }; break;
        }
        return true;
    }
}