#pragma once

#include <fstream>
#include <string_view>
#include <filesystem>
#include <array>
#include <vector>

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

namespace FileHandler{

using FsPath = std::filesystem::path;

class FileHandler{
public:
    FileHandler(std::fstream&& fileStream);

    FileHandler(const std::string& fileName, FileMode mode = FileMode::READ);
    
    FileHandler();

    ~FileHandler();
    
    FileHandler(const FileHandler&) = delete;

    FileHandler& operator=(const FileHandler&) = delete;

    // * Getters
    std::string GetFileName() const;
    bool IsFileOpen() const;
    bool IsFileEmpty();
    bool CreateFile(const std::string& fileName);
    void CloseFile();
    int DisplayChoice();
    int GetChoice() const;
    void OpenFile(const std::string& fileName, FileMode mode = FileMode::BINARY);
    std::vector<std::string> ReadFile(const std::string& file);
    std::string SetFileName();
    bool IsValidFileName(const std::string& fileName) const;
    bool SaveToFile(Fitter::Fitter* object, std::string_view fileName);
    void ClearScreen();

private:
    std::string m_fileName;
    std::fstream m_fileStream;
    bool m_isOpen = false;
    static unsigned int m_choice;    
};
} // namespace FileHandler