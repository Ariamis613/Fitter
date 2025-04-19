#pragma once
#include "App.h"
#include <fstream>
#include <string_view>
#include <filesystem>

namespace FileHandler{

class FileHandler{
public:
    explicit FileHandler(std::fstream& fileStream);
    FileHandler() = default;
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

    static bool FileHandler::ScanDirectoryForFile(std::string_view fileName, std::string_view directory);

public:
    void CreateFile(const std::string& fileName);
    void CloseFile();
    void OpenFile(const std::string& filename, FileMode mode = FileMode::BINARY);
    std::vector<std::string> ReadFile(const std::string& file);
    static bool MoveFile(std::string_view source, std::string_view destination);
    static bool DeleteFile(std::string_view fileName);

private:
    static std::string m_fileName;
    std::fstream& m_fileStream;
    std::string m_lastError;
    int* data = nullptr;
};
} // namespace FileHandler