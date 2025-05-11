#pragma once

#include <string>
#include <filesystem>
#include <stdexcept>

using FsPath = std::filesystem::path;

namespace utils{
    bool ScanDirectoryForFile(std::string_view fileName, const std::string& directory);
    bool MoveFile(std::string_view source, std::string_view destination);
    bool DeleteFile(std::string_view fileName);
    std::string GetSubdirectory();
    FsPath GetUserDirectory(std::string_view subdirectory);
} // namespace utils