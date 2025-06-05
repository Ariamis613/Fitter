#include "FileHandler.h"
#include "App.h"
#include "Utils.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <sys/stat.h>
#include <cstring>  // For strchr and strlen

namespace FileHandler{
    inline static constexpr int MAX_CHOICE = 6;
    inline static constexpr int MIN_CHOICE = 1;

    FileHandler::FileHandler(std::fstream&& fileStream) : m_fileStream(std::move(fileStream)) {
    }

    FileHandler::FileHandler(const std::string& fileName, FileMode mode) : m_fileName(""){
        try{
            OpenFile(fileName, mode);
            m_isOpen = true;
        } catch(const std::ios_base::failure& e){
            std::cerr << "Failed to open file. " << e.what() << std::endl;
        }   
    }

    FileHandler::FileHandler() = default;

    FileHandler::~FileHandler(){
        CloseFile();
    }
 
    void FileHandler::ClearScreen(){
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    std::string FileHandler::GetFileName() const{
        return m_fileName;
    }

    void FileHandler::CloseFile(){
        if(m_fileStream.is_open()){
            m_fileStream.close();
        }
    }

    bool FileHandler::IsFileOpen() const{
        return m_isOpen;
    }

    bool FileHandler::IsFileEmpty(){
        return m_fileStream.peek() == std::fstream::traits_type::eof();
    }

    int FileHandler::GetChoice() const{
        return m_choice;
    }

    int FileHandler::DisplayChoice(){
        std::cout << "Please select an option: \n"; 
        std::cout << "1. Create a new file.\n";
        std::cout << "2. Read from file.\n";
        std::cout << "3. Delete file.\n";
        std::cout << "4. Append to file.\n";
        std::cout << "5. Exit program.\n";
        std::cout << std::endl;

        std::cin >> m_choice;

        ClearScreen();

        return m_choice >= MIN_CHOICE && m_choice <= MAX_CHOICE ? m_choice : throw std::invalid_argument("Number should be between 1-3!\n");
    }

    void FileHandler::OpenFile(const std::string& fileName, const FileMode mode){
        CloseFile();

        std::ios::openmode openMode{};

        switch(mode){
            case FileMode::READ:
                openMode |= std::ios_base::in;
                break;
            case FileMode::WRITE:
                openMode |= std::ios_base::out | std::ios_base::trunc;
                break;
            case FileMode::APPEND:
                openMode |= std::ios_base::out | std::ios_base::app;
                break;
            case FileMode::READ_WRITE:
                openMode |= std::ios_base::in | std::ios_base::out;
                break;
            default:
                std::cerr << "Invalid file mode";
                break;
            }

        m_fileStream.open(fileName, openMode);
        
        if(m_fileStream.is_open()){
            m_fileName = fileName;
        }
        else{
            std::cerr << "Failed to open file: " + fileName;
            std::cout << std::endl;
        }
    }

    std::optional<std::vector<std::string>> FileHandler::ReadFile(const std::string& fileName){
        std::vector<std::string> lines_v{};
        
        try{
            // Close any previously opened file
            CloseFile();
            
            // Resolve the file path similar to CreateFile
            const auto userSubdirectory = Utils::GetSubdirectory();
            auto subdirectory = Utils::GetUserDirectory(userSubdirectory);
            const auto fullPath = subdirectory / fileName;
            
            // Open the file for reading with the full path
            OpenFile(fullPath.string(), FileMode::READ);
            
            if(!m_fileStream.is_open()){
                std::cerr << "Failed to open file: " << fullPath.string() << std::endl;
                return std::nullopt;
            }

            std::string buffer{};
            while(std::getline(m_fileStream, buffer)){
                lines_v.emplace_back(buffer);
            }
            
            // Check if we reached the end of file properly
            if(m_fileStream.eof()){
                // Clear EOF flag
                m_fileStream.clear();
            } else if(m_fileStream.fail() && !lines_v.empty()) {
                // Some error occurred during reading, but we got some lines
                std::cerr << "Warning: Error while reading file (some data may be missing)" << std::endl;
            }
            return lines_v;
        }
        catch(const std::exception& e){
            std::cerr << "Error reading file: " << e.what() << std::endl;
            return std::nullopt;
        }
    }

    // Saves data to an existing file
    bool FileHandler::SaveToFile(Fitter::Fitter* object, FsPath_t filePath){
        if(object == nullptr){
            std::cerr << "Cannot save null object!" << std::endl;
            return false;
        }

        std::string baseFileName;
        // Determine which filename to use
        if(!filePath.empty()){
            baseFileName = filePath.filename().string();
        } else if(!m_fileName.empty()) {
            baseFileName = m_fileName;
        } else {
            std::cerr << "No filename specified!" << std::endl;
            return false;
        }
        
        try{
            // Resolve the full path similar to other functions
            //const auto userSubdirectory = Utils::GetSubdirectory();
            //auto subdirectory = Utils::GetUserDirectory(userSubdirectory);
            const auto fullPath = m_referencePath;
            
            // Check if file exists
            if(!std::filesystem::exists(fullPath)){
                std::cerr << "File does not exist: " << fullPath.string() << std::endl;
                std::cerr << "Use CreateFile() first or provide a valid file path." << std::endl;
                return false;
            }
            
            // Use OpenFile instead of directly creating std::ofstream
            OpenFile(fullPath.string(), FileMode::WRITE);
            
            if(!m_fileStream.is_open()){
                std::cerr << "Failed to open file for writing: " << fullPath.string() << std::endl;
                return false;
            }

            auto time = object->GetNow();

            m_fileStream << "Exercise: "  << object->GetExcersiseName() << ", Sets: "
                    << object->GetSets() << ", Reps: " << object->GetReps()
                    << ", Weight: " << object->GetWeightKG() << "kg, lbs: "
                    << object->GetWeightLBS() << ", Time: " << std::asctime(time)
                    << std::endl;
                    
            m_fileStream << std::endl;
            
            // Close the file
            CloseFile();

            // Update m_fileName to the base filename
            m_fileName = baseFileName;

            std::cout << "Data saved successfully to: " << fullPath.string() << std::endl;
            return true;
        } catch(const std::exception& e){
            std::cerr << "Error saving file: " << e.what() << std::endl;
            return false;
        }
    }

    bool FileHandler::CreateFile(const std::string& fileName){
        CloseFile();

        const auto userSubdirectory = Utils::GetSubdirectory();
        auto subdirectory = Utils::GetUserDirectory(userSubdirectory);

        try{
            if(std::filesystem::exists(fileName)){
                std::cerr << "File already exists!" << std::endl;
            }

            const auto fullPath = subdirectory / fileName;

            std::ofstream newFile(fullPath);
            
            
            
            if(!newFile.is_open()){
                std::cerr << "Failed to create file: " + fullPath.string();
            }

            m_referencePath = fullPath;

            newFile.close();
            
            std::cout << "File created at: " << fullPath << std::endl;
            return true;
        }
        catch(const std::exception& e){
            std::cerr << "Error:" << e.what() << std::endl;
            return false;
        }
    }

    bool FileHandler::IsValidFileName(const char* fileName){
        if(fileName == nullptr || fileName[0] == '\0'){
            std::cerr << "File name cannot be empty!" << std::endl;
            return false;
        }

        static constexpr std::array<char, 9> invalidChars = {
            '\\', '/', ':', '*', '?', '\"', '<', '>', '|'
        };

        // Check for invalid characters
        for(const auto invalidChar : invalidChars){
            if(strchr(fileName, invalidChar) != nullptr){
                std::cerr << "File name contains invalid characters!" << std::endl;
                return false;
            }
        }

        // Check for leading/trailing spaces or periods
        size_t len = strlen(fileName);
        if (fileName[0] == ' ' || fileName[len-1] == ' ' ||
            fileName[0] == '.' || fileName[len-1] == '.'){
            std::cerr << "File name cannot start or end with spaces or periods!" << std::endl;
            return false;
        }
        return true;
    }

    // Gets and sets the file name based on user input
    std::string FileHandler::SetFileName(){
        std::string fileName{};

        // Clear any leftover characters in the input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "Enter filename: ";
        std::getline(std::cin, fileName);
        
        if(!IsValidFileName(fileName.c_str())){
            std::cerr << "Filename is invalid!" << std::endl;
            return "";
        }
        
        m_fileName = fileName;
        return m_fileName;
    }

    void FileHandler::DeleteFile(const char* filePath){
        const auto userSubdirectory = Utils::GetSubdirectory();
        auto subdirectory = Utils::GetUserDirectory(userSubdirectory);
        const auto fullPath = subdirectory / filePath;

    try{
    if(std::filesystem::exists(fullPath)){
      std::filesystem::remove(fullPath);
      std::cout << "File: " << filePath << " deleted successfully." << std::endl;

    }else{
      std::cout << "File: " << filePath << " doesn't exist." << std::endl;
      return;
    }
    } catch(std::exception& e){
      std::cerr << "Error deleting file: " << e.what() << std::endl;
      return;
    }
  }

    bool FileHandler::AppendToFile(Fitter::Fitter* object, FsPath_t filePath){
        if(object == nullptr || filePath.empty()){
            std::cerr << "Cannot append null object or empty file path!";
            return false;
        }

        try{
            const auto userSubdirectory = Utils::GetSubdirectory();
            auto subdirectory = Utils::GetUserDirectory(userSubdirectory);
            const auto fullPath = subdirectory / filePath.filename().string();
            
            if(!std::filesystem::exists(fullPath)){
                std::cerr << "File does not exist: " << fullPath.string() << std::endl;
                std::cerr << "Use CreateFile() first or provide a valid file path." << std::endl;
                return false;
            }
            
            OpenFile(fullPath.string(), FileMode::APPEND);
            
            if(!m_fileStream.is_open()){
                std::cerr << "Failed to open file for writing: " << fullPath.string() << std::endl;
                return false;
            }

            auto time = object->GetNow();

            m_fileStream << "Exercise: "  << object->GetExcersiseName() << ", Sets: "
                    << object->GetSets() << ", Reps: " << object->GetReps()
                    << ", Weight: " << object->GetWeightKG() << "kg, lbs: "
                    << object->GetWeightLBS() << ", Time: " << std::asctime(time)
                    << std::endl;
                    
            m_fileStream << std::endl;
            
            CloseFile();

            m_fileName = filePath.filename().string();

            printf("Data saved successfully to: %s", fullPath.c_str());
            return true;
        } catch(const std::exception& e){
            std::cerr << "Error saving file: " << e.what() << std::endl;
            return false;
        }
    }
} //namespace FileHandler