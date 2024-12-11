#include "File.hpp"
#include "Logger.hpp"

#include <utility/Assert.hpp>


namespace Ruby {
    namepace Details::IO {
        
    }



    SharedPtr<byte> readAllData(FILE* file, size_t fileSize) {
        RUBY_ASSERT_BASIC(file != nullptr);

        byte* buffer = new(std::nothrow) byte[fileSize + 1];
        if (!buffer) {
            return nullptr;
        }

        auto readed = fread(buffer, sizeof(byte), fileSize, file);
        if (readed != fileSize) {
            return nullptr;
        }

        buffer[fileSize] = '\0';
        return makeShared<byte>(buffer, [](byte* ptr) { 
            delete[] ptr; 
        });
    }



    Opt<String> File::ReadStatic(std::string_view path) {
        File f{ path, "r" };
        return f.ReadAsString();
    }

    SharedPtr<byte> File::ReadBinaryStatic(std::string_view path) {
        File f{ path, "rb" };
        return f.ReadAsBytes();
    }

    File&& File::OpenStatic(std::string_view path, std::string_view mode, bool abortOnError=false) {
        return File{ path, mode, abortOnError };
    }




    File::File(std::string_view path, std::string_view mode, bool abortOnError) {
        if (!Open(path, mode, abortOnError)) {

        }

        m_mode = mode;
    }

    bool File::IsOpened() const {
        return m_file != nullptr;
    }

    bool File::OnBegin() const {
        return Tell() == 0;
    }

    size_t File::Tell() const {
        return ftell(m_file);
    }

    i32 File::SeekCur(size_t offset) {
        return fseek(m_file, offset, SEEK_CUR);
    }

    i32 File::SeekBegin(size_t offset) {
        return fseek(m_file, offset, SEEK_SET);
    }

    i32 File::SeekEnd(size_t offset) {
        return fseek(m_file, offset, SEEK_END);
    }


    bool File::Write(std::string_view data) const {
        RUBY_ASSERT_BASIC(m_file != nullptr);

        fwrite(data.data(), sizeof(std::string_view::value_type), data.size(), m_file);
    }

    bool File::Open(std::string_view path, std::string_view mode, bool abortOnError=false) {
        if (fopen_s(&m_file, path.data(), mode.data())) {
            if (abortOnError)
                RUBY_CRITICAL("File::Open() : Failed to open file \"{}\"", path.data());
            RUBY_ERROR("File::Open() : Failed to open file \"{}\". abortOnError = false", path.data());
            return false;
        }

        SeekEnd(0);
        m_fileSize = Tell();
        Rewind();

        return true;
    }

    Opt<String> File::ReadAsString(bool rewindOnEnd) {
        RUBY_ASSERT_BASIC(m_file != nullptr);

        auto data = readAllData(m_file, m_fileSize);
        if (!data) {
            RUBY_ERROR("File::ReadAll() : Failed to read all data from file");
            return nullopt;
        }

        if (rewindOnEnd) {
            Rewind();
        }

        return String{ (const char*)data.get() };
    }

    SharedPtr<byte> File::ReadAsBytes(bool rewindOnEnd) {
        RUBY_ASSERT_BASIC(m_file != nullptr);

        auto bytes = readAllData(m_file, m_fileSize);
        if (rewindOnEnd) {
            Rewind();
        }        

        return bytes;
    }



    Opt<String> File::ReadLineAsString() {
        RUBY_ASSERT_BASIC(m_file != nullptr);

        char* line = nullptr;
        size_t lineLength = 0;

        if ((getline(&line, &lineLength, m_file)) == -1)
            return nullopt;

        return String{ line };
    }

    SharedPtr<byte> File::ReadLineAsBytes() const {
        RUBY_ASSERT_BASIC(m_file != nullptr);

        SharedPtr<byte> line = nullptr;
    }

    void File::Rewind() {
        rewind(m_file);
    }

    u32 File::GetFileSize() const {
        return m_fileSize;
    }

    std::string_view File::GetOpenMode() const {
        return m_mode;
    }

    File::~File() {
        fclose(m_file);
    }
}