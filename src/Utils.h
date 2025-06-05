#pragma once

#include <string>
#include <filesystem>
#include <stdexcept>

using FsPath_t = std::filesystem::path;

// Replace macros with inline constexpr variables
inline constexpr double POUNDS_TO_KG = 0.45359237;
inline constexpr double KG_TO_POUNDS = 2.20462262;

class Utils{
public:
    static bool ScanDirectoryForFile(std::string_view fileName, const std::string& directory);
    static bool MoveFile(std::string_view source, std::string_view destination);
    static bool DeleteFile(std::string_view fileName);
    static std::string GetSubdirectory();
    static FsPath_t GetUserDirectory(std::string_view subdirectory);

    template<typename T>
    static double ConvertToKg(T lbsValue){
        return lbsValue * POUNDS_TO_KG;
    }

    template<typename T>
    static double ConvertToLbs(T kgValue){
        return kgValue * KG_TO_POUNDS;
    }

    // Calculate total volume (sets × reps × weight)
    template<typename T>
    static T CalculateVolume(unsigned int sets, unsigned int reps, double weight){
        return static_cast<T>(sets * reps * weight);
    }
};