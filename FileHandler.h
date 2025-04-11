#pragma once
#include "App.h"
#include <fstream>
#include <string_view>
#include <filesystem>

namespace FileHandler{

class FileHandler{
public:
    explicit FileHandler(std::fstream& fileStream);
    ~FileHandler();
    
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    enum class FileMode{
        READ,
        WRITE,
        APPEND,
        READ_WRITE,
        BINARY,
    };

    std::string GetFileName() const;

    bool isFileOpen() const;

    bool FileHandler::ScanDirectoryForFile(const std::string_view fileName, const std::string_view directory);

    // @ari: TODO: Implement these
public:
    void CreateFile();
    bool FileExists(const std::string& fileName, const std::string_view directory);
    void CloseFile();
    void OpenFile(const std::string& filename, FileMode mode);
    std::string ReadFile(const std::string& file);
    bool MoveFile(const std::string_view source, const std::string_view destination);

private:
    static std::string m_fileName;
    std::fstream& m_fileStream;
    std::string m_lastError;
    int* data;
};
} // namespace FileHandler