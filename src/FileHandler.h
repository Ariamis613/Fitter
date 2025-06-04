#pragma once

#include <fstream>
#include <string_view>
#include <filesystem>
#include <array>
#include <vector>
#include <optional>

enum class FileMode{
    READ,
    WRITE,
    APPEND,
    READ_WRITE,
    BINARY,
};

namespace Fitter{
    class Fitter;
}

using FsPath_t = std::filesystem::path;

namespace FileHandler{
class FileHandler{
public:
    FileHandler(std::fstream&& fileStream);

    FileHandler(const std::string& fileName, FileMode mode = FileMode::READ);
    
    FileHandler();

    virtual ~FileHandler();
    
    FileHandler(const FileHandler&) = delete;

    FileHandler& operator=(const FileHandler&) = delete;

    bool CreateFile(const std::string& fileName); 
    bool SaveToFile(Fitter::Fitter* object, FsPath_t file); 
    std::optional<std::vector<std::string>> ReadFile(const std::string& fileName);
    bool AppendToFile(Fitter::Fitter* object, FsPath_t file);
    void DeleteFile(const char* filePath);
    
    std::string GetFileName() const;
    std::string SetFileName();
    bool IsFileOpen() const;
    bool IsFileEmpty();
    bool IsValidFileName(const char* fileName);
    void CloseFile();
    int DisplayChoice();
    int GetChoice() const;
    void OpenFile(const std::string& fileName, FileMode mode = FileMode::READ);
    static void ClearScreen();

private:
    FsPath_t m_referencePath;
    std::string m_fileName;
    std::fstream m_fileStream;
    bool m_isOpen = false;
    unsigned int m_choice{0};
};
} // namespace FileHandler