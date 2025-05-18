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
        std::cout << "2. Save to file.\n";
        std::cout << "3. Read from file.\n";
        std::cout << std::endl;

        std::cin >> m_choice;

        ClearScreen();

        return m_choice > 0 && m_choice <= 3 ? m_choice : throw std::invalid_argument("Number should be between 1-3!\n");
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

    // @ari: Debating whether to make this a void or keep it that way
    std::vector<std::string> FileHandler::ReadFile(const std::string& file){

        constexpr std::ios_base::openmode mode = std::ios_base::in;

        std::vector<std::string> lines_v{};
        
        std::string buffer{};

        try{
            while(std::getline(m_fileStream, buffer)){
                lines_v.emplace_back(buffer);
            }
            return lines_v;
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
            return {};
        }
    }

    // Saves data to an existing file
    bool FileHandler::SaveToFile(Fitter::Fitter* object, FsPath filePath){
        if(object == nullptr){
            std::cerr << "Cannot save null object!" << std::endl;
            return false;
        }

        // Use provided filePath or fall back to m_fileName if available
        std::string targetFile = filePath.empty() ? m_fileName : filePath.string();
        
        if(targetFile.empty()){
            std::cerr << "No filename specified!" << std::endl;
            return false;
        }

        try{
            // Check if file exists
            if(!std::filesystem::exists(targetFile)){
                std::cerr << "File does not exist: " << targetFile << std::endl;
                std::cerr << "Use CreateFile() first or provide a valid file path." << std::endl;
                return false;
            }
            
            // Open file for writing
            std::ofstream outFile(targetFile);
            if(!outFile.is_open()){
                std::cerr << "Failed to open file for writing: " << targetFile << std::endl;
                return false;
            }

            auto time = object->GetNow();

            outFile << "Exercise: "  << object->GetExcersiseName() << ", Sets: "
                    << object->GetSets() << ", Reps: " << object->GetReps()
                    << ", Weight: " << object->GetWeightKG() << "kg, lbs: "
                    << object->getWeightLBS() << ", Time: " << std::asctime(time)
                    << std::endl;
                    
            outFile << std::endl;
            outFile << "SaveFile() called" << std::endl;
            outFile.close();

            // Update m_fileName if we used a new path
            if(!filePath.empty() && m_fileName != targetFile){
                m_fileName = targetFile;
            }

            std::cout << "Data saved successfully to: " << targetFile << std::endl;
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

            // std::filesystem::path works the same on Windows and Linux
            const auto fullPath = subdirectory / fileName;

            std::ofstream newFile(fullPath);

            if(!newFile.is_open()){
                std::cerr << "Failed to create file: " + fullPath.string();
            }

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
} //namespace FileHandler