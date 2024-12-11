#pragma once

#include "TypeTraits.hpp"
#include <utility/Definitions.hpp>


namespace Ruby {
    class File {
    public:
        static bool WriteStatic(std::string_view data);

        static Opt<String> ReadStatic(std::string_view path);
        static SharedPtr<byte> ReadBinaryStatic(std::string_view path);

        static File&& OpenStatic(std::string_view path, std::string_view mode, bool abortOnError=false);

    public:
        File() = default;
        File(std::string_view path, std::string_view mode, bool abortOnError=false);

        bool Open(std::string_view path, std::string_view mode, bool abortOnError=false);

        bool IsOpened() const;
        bool IsEOF() const;
        bool OnBegin() const;

        size_t Tell() const;

        i32 SeekCur(size_t offset);
        i32 SeekBegin(size_t offset);
        i32 SeekEnd(size_t offset);

        bool Put(char ch) const;
        bool Write(std::string_view data) const;

        Opt<String> ReadAsString(bool rewindOnEnd=false);
        SharedPtr<byte> ReadAsBytes(bool rewindOnEnd=false);

        void Rewind();

        Opt<String> ReadLineAsString();
        SharedPtr<byte> ReadLineAsBytes() const;

        u32 GetFileSize() const;
        std::string_view GetOpenMode() const;

        ~File();
          
    private:
        FILE* m_file = nullptr;
        std::string_view m_mode;
        size_t m_fileSize = 0;
    };
}