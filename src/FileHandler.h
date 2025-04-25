#pragma once

#include "App.h"

#include <fstream>
#include <string_view>
#include <filesystem>
#include <array>

using FsPath = std::filesystem::path;

namespace FileHandler{

    enum class FileMode{
        READ,
        WRITE,
        APPEND,
        READ_WRITE,
        BINARY,
    };

class FileHandler{
public:
    FileHandler(std::fstream&& fileStream);
    FileHandler(const std::string& fileName, FileMode mode = FileMode::READ);
    ~FileHandler();
    
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    // * Getters
    std::string GetFileName() const;

    bool IsFileOpen() const;
    bool IsFileEmpty();
    static bool ScanDirectoryForFile(std::string_view fileName, const std::string& directory);
    bool CreateFile(const std::string& fileName);
    void CloseFile();
    void OpenFile(const std::string& filename, FileMode mode = FileMode::BINARY);
    std::vector<std::string> ReadFile(const std::string& file);
    
protected:
    static bool MoveFile(std::string_view source, std::string_view destination);
    static bool DeleteFile(std::string_view fileName);
    static std::string GetSubdirectory();
    FsPath GetUserDirectory(std::string_view subdirectory) const;

private:
    std::string m_fileName;
    std::fstream m_fileStream;
};
} // namespace FileHandler

namespace JSON{

// TODO: Implement JSON support for FileHandling
class JSON : public FileHandler::FileHandler{
public:
    JSON(const std::string& fileName, const std::string& fileExt = ".json");
    ~JSON();

private:

};

} // namespace CSV