#include "Utils.h"

#include <algorithm>
#include <array>
#include <iostream>

static constexpr std::array<char, 4> directoryDelimiters = {
  '\\', '/', ':','|'
};

namespace utils {

// * @NOTE: Checks if the file exists in the directory //
bool ScanDirectoryForFile(std::string_view fileName, const std::string& directory) {
  FsPath dirPath(directory);

  if (!std::filesystem::is_directory(directory)) {
    std::cerr << "Provided directory is invalid\n";
  }
  FsPath filePath(fileName);
  if (filePath.has_parent_path() || filePath.is_absolute()) {
    std::cerr << "Invalid file name\n";
  }
  std::cout << std::endl;

  const FsPath fullPath = dirPath / filePath;

  return std::filesystem::exists(fullPath) &&
         std::filesystem::is_regular_file(fullPath);
}

bool MoveFile(std::string_view source, std::string_view destination) {
  try {
    std::filesystem::rename(source, destination);
    return true;
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}

bool DeleteFile(std::string_view fileName) {
  try {
    return std::filesystem::remove(fileName);
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}

std::string GetSubdirectory() {
  std::string subdirectory{};

  while (true) {
    std::cout << "Enter the subdirectory name (e.g 'Desktop'): ";
    std::getline(std::cin, subdirectory);

    if (subdirectory.empty()) {
      std::cerr << "Subdirectory name cannot be empty!" << std::endl;
      continue;
    }
    if (std::any_of(directoryDelimiters.begin(), directoryDelimiters.end(),
                    [&subdirectory](char delim) {
                      return subdirectory.find(delim) != std::string::npos;
                    })) {
      std::cout << "Invalid subdirectory name. Please avoid using '/', '\\', "
                   "'..', or ':'."
                << std::endl;
    }
    break;
  }
  return subdirectory;
}

FsPath GetUserDirectory(std::string_view subdirectory) {
  FsPath directory{};

  try {
#ifdef _WIN32  // WINDOWS
    const char* homeDir = std::getenv("USERPROFILE");
    if (!userProfile) {
      std::cerr << "Failed to resolve user profile directory!" << std::endl;
    }
    directory = homeDir;
#else  // LINUX/macOS
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) {
      std::cerr << "Failed to resolve user profile directory!" << std::endl;
    }
    directory = homeDir;
#endif

    FsPath targetDir = directory / subdirectory;

    if (!std::filesystem::exists(targetDir)) {
      try {
        std::filesystem::create_directory(targetDir);
        std::cout << "Created directory: " << targetDir << std::endl;
      } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to create directory " << e.what() << '\n';
        std::cout << "Going back to default directory: 'Documents'... "
                  << std::endl;

        FsPath docsDir = directory / "Documents";
        if (std::filesystem::exists(docsDir)) {
          return docsDir;
          std::cout << "Default directory set: 'Documents'" << std::endl;
        }
        return directory;
      }
    }
    return targetDir;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return directory;
  }
}
}  // namespace utils