#include "RubyUtility.hpp"

#include <types/Logger.hpp>


namespace Ruby {
    String extractTextFromFile(std::string_view path) {
        FILE* file = nullptr;

        if (fopen_s(&file, path.data(), "rb"))
            RUBY_CRITICAL("extractDataFromFile() : failed to open file \"{}\" (fopen_s returns non-zero value)", path);

        fseek(file, 0, SEEK_END);
        auto fsize = ftell(file);
        rewind(file);

        byte* buffer = new(std::nothrow) byte[fsize + 1];
        if (!buffer) {
            RUBY_ERROR("extractDataFromFile() : Failed to allocate memory for buffer");
            return String{ "" };
        }

        auto bytesRead = fread(buffer, sizeof(byte), fsize, file);
        if (bytesRead != fsize)
            RUBY_CRITICAL("extractDataFromFile() : failed to read all necessary bytes from file \"{}\" (bytes read: {}, expected: {})",
                          path, bytesRead, fsize);
        fclose(file);

        buffer[fsize] = '\0';

        return String { (char*)buffer };
    }


    Opt<bool> strToBool(const char* str) {
        String lowerStr{ str };
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), tolower);

        return lowerStr == "true";
    }
}