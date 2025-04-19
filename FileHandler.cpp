#include "FileHandler.h"

#include <stdexcept>
#include <iostream>
#include <string>
#include <sys/stat.h>



namespace FileHandler{
    
    FileHandler::FileHandler(std::fstream& fileStream) : m_fileStream(fileStream) {}

    FileHandler::~FileHandler(){
        CloseFile();
    }

    std::string FileHandler::GetFileName() const{
        return m_fileName;
    }

    void FileHandler::CloseFile(){
        if(m_fileStream.is_open()){
            m_fileStream.close();
        }
    }

    bool FileHandler::isFileOpen() const{
        return m_fileStream.is_open();
    }

    void FileHandler::OpenFile(const std::string& filename, FileMode mode){
        CloseFile();

        std::ios::openmode openMode;

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
            case FileMode::BINARY:
                openMode |= std::ios_base::binary;
                break;
            default:
                throw std::ios_base::failure("Invalid file mode");
                break;
        }

        m_fileStream.open(filename, openMode);
        
        if(m_fileStream.is_open()){
            m_fileName = filename;
        }
        else{
            throw std::ios_base::failure("Failed to open file: " + filename);
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
            std::cout << e.what() << std::endl;
            return lines_v;
        }
    }

    bool FileHandler::ScanDirectoryForFile(std::string_view fileName, const std::string& directory){

        std::filesystem::path dirPath(directory);

        if(!std::filesystem::is_directory(directory)){
            throw std::invalid_argument("Provided directory is invalid"); 
        }
        std::filesystem::path filePath(fileName);
        if(filePath.has_parent_path() || filePath.is_absolute()){
            throw std::invalid_argument("Invalid file name");
        }

        const std::filesystem::path fullPath = dirPath / filePath;

        return std::filesystem::exists(fullPath) && std::filesystem::is_regular_file(fullPath);
    }

    std::string_view GetSubdirectory(){


        std::string subdirectory{};
        while(true){
            std::cout << "Enter the subdirectory name: (e.g 'Desktop')";
            std::getline(std::cin, subdirectory);

            if(subdirectory.empty()){
                std::cerr << "Subdirectory name cannot be empty!" << std::endl;
                continue;
            }

            for(const auto& delim : directoryDelimiters){
                if(subdirectory.find(delim) != std::string::npos){
                    std::cout << "Invalid subdirectory name. Please avoid using '/', '\\', '..' or ':'" << std::endl;
                    continue;
                }
            }

            break;

        }

        return subdirectory;

    }

    std::filesystem::path FileHandler::GetUserDirectory(std::string_view subdirectory) const{
        //@NOTE: TODO
        std::filesystem::path directory{};
        std::string_view defaultDir = "Documents";

        #ifdef _WIN32
        const char* userProfile = std::getenv("USERPROFILE");
        if(!userProfile){
            throw std::runtime_error("Failed to resolve user profile directory!");
        }
        if(!std::filesystem::exists(defaultDir)){
            directory = std::filesystem::path(userProfile) / subdirectory;
            return directory;
        }
        #else
        const char* userProfile = std::getenv("HOME");
        if(!userProfile){
            throw std::runtime_error("Failed to resolve user profile directory!");      
        }
        if(!std::filesystem::exists(defaultDir)){
            directory = std::filesystem::path(userProfile) / subdirectory;
            return directory;
        }
        #endif
    }

    bool FileHandler::CreateFile(const std::string& fileName){
        CloseFile();

        std::string_view userSubdirectory = GetSubdirectory();
        std::filesystem::path subdirectory = GetUserDirectory(userSubdirectory);

        try{
            if(std::filesystem::exists(fileName)){
                throw std::runtime_error("File already exists!");
            }

            #ifdef _WIN32
            const std::filesystem::path fullPath = subdirectory / fileName;
            #else
            const std::filesystem::path fullPath = subdirectory / fileName;
            #endif

            std::ofstream newFile(fullPath);

            if(!newFile.is_open()){
                throw std::runtime_error("Failed to create file: " + fullPath.string());
            }
            
            std::cout << "File created successfully!: " << fullPath << std::endl;
            return true;
        }
        catch(const std::exception& e){
            std::cerr << "Error:" << e.what() << std::endl;
            return false;
        }
    }

     bool FileHandler::MoveFile(std::string_view source, std::string_view destination){
        try{
            std::filesystem::rename(source, destination);
            return true;
        }
        catch(const std::exception& e){
            std::cout << e.what() << std::endl;
            return false;
        }
    }

    bool FileHandler::DeleteFile(std::string_view fileName){
        try{
            return std::filesystem::remove(fileName);
        }
        catch(const std::exception& e){
            std::cout << e.what() << std::endl;
        }
    }
} //namespace FileHandler