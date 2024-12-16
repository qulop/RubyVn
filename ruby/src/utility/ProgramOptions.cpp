#include <platform/Platform.hpp>    // Logger doesn't initialize at this moment, so we need to use writeInConsoleF() or Platform::writeInConsole()

#include "ProgramOptions.hpp"
#include "Assert.hpp"
#include "Cast.hpp"


namespace Ruby {
    static std::string_view argTypeToStringView(OptionArgType type) {
        switch (type) {
            case OptionArgType::INT:
                return "int";
            case OptionArgType::BOOL:
                return "bool";
            case OptionArgType::STRING:
                return "string";
            default:
                return "none";
        }
    }

    static OptionArgType getArgumentType(const char* arg) {
        RUBY_ASSERT_BASIC(arg && !ProgramOptions::IsFlag(arg));

        if (std::strcmp(arg, "true") == 0 || std::strcmp(arg, "false") == 0)
            return OptionArgType::BOOL;
        else if (strToInt<i32>(arg).has_value())
            return OptionArgType::INT;
        return OptionArgType::STRING;
    }

    static bool checkTypeOfArgument(const CmdLineOption& opt, const char* arg) {
        if (!arg || ProgramOptions::IsFlag(arg)) {
            writeInConsoleF("Missing argument for option \"--{}\"\n",
               opt.longName);
            return false;
        }

        if (auto deducedType = getArgumentType(arg); deducedType != opt.type) {
            writeInConsoleF("Option \"--{}\" expects argument of type \"{}\", but gets \"{}\"\n",
                opt.longName,
                argTypeToStringView(opt.type),
                argTypeToStringView(deducedType)
            );
            return false;
        }

        return true;
    }



    ProgramOptions::ProgramOptions(i32 argc, char** argv, std::initializer_list<CmdLineOption> opts) :
            m_argc(argc - 1),   // Excluding first argument(application path)
            m_appPath(argv[0])
    {
        std::lock_guard<std::mutex> guard{ m_parseMutex };

        CopyRawOptions(argv);
        if (!m_argv) {
            return;
        }

        auto mandatoryOptionsTable = std::move(CreateTableOfMandatoryOptions(opts.begin(), opts.end()));
        for (size_t tokenIndex = 0; tokenIndex < m_argc; tokenIndex++) {
            String token = At(tokenIndex);

            if (!IsFlag(token)) {
                writeInConsoleF("Argument doesn't apply to any flag: \"{}\"\n", token);
                continue;
            }

            if (!ExtractOptionName(token) || !IsOptionExistsInTable(mandatoryOptionsTable, token)) {
                AbortParse();
                return;
            }

            const auto& option = mandatoryOptionsTable.at(token);
            if (option.type == OptionArgType::NONE) {
                m_options[option.longName] = std::monostate{};
                continue;
            }

            tokenIndex += 1;
            const char* argument = (tokenIndex < m_argc) ? At(tokenIndex) : nullptr;
            if (!ParseArgument(option, argument)) {
                AbortParse();
                return;
            }
        }

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

    bool ProgramOptions::IsFlag(std::string_view arg) {
        if (arg.empty()) {
            return false;
        }

        return (arg.size() > 2 && arg.starts_with("--")) || (arg.size() > 1 && arg.starts_with("-"));
    }

    char* ProgramOptions::At(size_t i) {
        return operator[](i);
    }

    char* ProgramOptions::operator[](size_t i) {
        RUBY_ASSERT(i < m_argc, "Index out of borders");

        return m_argv[i];
    }

    bool ProgramOptions::HasOption(const String& opt) const {
        return m_options.contains(opt);
    }

    std::any ProgramOptions::GetArgumentOfOption(const String& opt) const {
        if (!HasOption(opt) || std::holds_alternative<std::monostate>(m_options.at(opt))) {
            return std::any{};
        }

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

    String ProgramOptions::GetAppPath() const {
        return m_appPath;
    }


    ProgramOptions& ProgramOptions::operator=(const ProgramOptions& other) {
        if (this == &other) {
            return *this;
        }

        m_argc = other.m_argc;
        m_appPath = other.m_appPath;
        CopyRawOptions(other.m_argv);
        m_options = other.m_options;
        m_isParseProcessed.exchange(other.m_isParseProcessed.load());

        return *this;
    }

    ProgramOptions& ProgramOptions::operator=(ProgramOptions&& other) noexcept {
        if (this == &other) {
            return *this;
        }

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
        RUBY_ASSERT_BASIC(args != nullptr);

        i32 argc = m_argc + 1;
        m_argv = new(std::nothrow) char*[argc];  // content of args(without path) + nullptr limiter
        if (!m_argv) {
            writeInConsoleF("Failed to allocate memory for the m_argv");
            return;
        }

        for (auto i = 1; i < argc; i++) {
            size_t len = std::strlen(args[i]) + 1;
            m_argv[i-1] = new char[len];
            strcpy_s(m_argv[i-1], len, args[i]);
        }

        m_argv[argc - 1] = nullptr;
    }

    bool ProgramOptions::ExtractOptionName(String& arg) const {
        size_t beginOfFlagName = arg.find_first_not_of('-');
        if (beginOfFlagName == String::npos) {
            writeInConsoleF("Failed to find name of option: \"{}\"\n", arg);
            return false;
        }

        arg.erase(0, beginOfFlagName);
        return true;
    }

    void ProgramOptions::AddRemainingRequiredOptions(auto begin, auto end) {
        for (auto opt = begin; opt != end; opt++) {
            bool isDefaultValueExists = !std::holds_alternative<std::monostate>(opt->defaultValue);

            if (m_options.contains(opt->longName) || !isDefaultValueExists)
                continue;
            m_options[opt->longName] = opt->defaultValue;
        }
    }

    bool ProgramOptions::IsOptionExistsInTable(const ProgramOptions::OptionsMapType& mandatoryOptionsTable, const String& flag) const {
        if (mandatoryOptionsTable.contains(flag)) {
            return true;
        }

        writeInConsoleF("Unknown option is found: \"{}\"\n", flag);
        return false;
    }

    bool ProgramOptions::ParseArgument(const CmdLineOption& option, const char* argument) {
        if (!checkTypeOfArgument(option, argument)) {
            return false;
        }

        switch (option.type) {
            case OptionArgType::INT:
                m_options[option.longName] = strToInt<i32>(argument).value(); break;
            case OptionArgType::BOOL:
                m_options[option.longName] = strToBool(argument).value(); break;
            case OptionArgType::STRING:
                m_options[option.longName] = argument; break;
            default:
                RUBY_WRECK("Default case was reached, but it was not intended");
        }

        return true;
    }

    void ProgramOptions::AbortParse() {
        auto tmp = std::move(*this);    // Will reset all fields of this object
    }   // Here m_argv will be deleted in the destructor

    ProgramOptions::OptionsMapType ProgramOptions::CreateTableOfMandatoryOptions(auto begin, auto end) const {
        OptionsMapType ret;
        for (auto opt = begin; opt != end; opt++) {
            ret[opt->longName] = *opt;
        }

        return ret;
    }
}