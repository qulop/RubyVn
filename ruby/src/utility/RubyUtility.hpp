#pragma once

#include <types/TypeTraits.hpp>
#include <types/Singleton.hpp>
#include <types/Logger.hpp>

#include "Definitions.hpp"
#include "Enum.hpp"


namespace Ruby {
      RUBY_FORCEINLINE FILE* openFile(const char* path, const char* mode, bool abortOnError=true) {
        FILE* file = nullptr;
        if (!fopen_s(&file, path, mode))
            return file;

        if (abortOnError)
            RUBY_CRITICAL("openFile() : Failed to open file \"{}\"", path);

        RUBY_ERROR("openFile() : Failed to open file \"{}\". abortOnError = false", path);
    }

    inline String extractTextFromFile(std::string_view path) {
        FILE* file = openFile(path.data(), "rb");

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
}