#pragma once

#include <string>
#include <filesystem>
#include <stdexcept>
#include <type_traits>

using FsPath = std::filesystem::path;

namespace utils{
    bool ScanDirectoryForFile(std::string_view fileName, const std::string& directory);
    bool MoveFile(std::string_view source, std::string_view destination);
    bool DeleteFile(std::string_view fileName);
    std::string GetSubdirectory();
    FsPath GetUserDirectory(std::string_view subdirectory);

    template<typename T>
    double ConvertToKg(T lbsValue){
        return lbsValue * 0.45359237;
    }

    template<typename T>
    double ConvertToLbs(T kgValue){
        return kgValue * 2.20462262;
    }
} // namespace utils