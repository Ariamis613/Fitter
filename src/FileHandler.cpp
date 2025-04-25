#include "FileHandler.h"

#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <algorithm>

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
            throw std::runtime_error(e.what());
            return {};
        }
    }

    bool FileHandler::ScanDirectoryForFile(std::string_view fileName, const std::string& directory){

        FsPath dirPath(directory);

        if(!std::filesystem::is_directory(directory)){
            throw std::invalid_argument("Provided directory is invalid"); 
        }
        FsPath filePath(fileName);
        if(filePath.has_parent_path() || filePath.is_absolute()){
            throw std::invalid_argument("Invalid file name");
        }

        const FsPath fullPath = dirPath / filePath;

        return std::filesystem::exists(fullPath) && std::filesystem::is_regular_file(fullPath);
    }

    std::string FileHandler::GetSubdirectory(){

        std::string subdirectory{};
        
        while(true){
            std::cout << "Enter the subdirectory name: (e.g 'Desktop')";
            std::getline(std::cin, subdirectory);

            if(subdirectory.empty()){
                std::cerr << "Subdirectory name cannot be empty!" << std::endl;
                continue;
            }
            if(std::any_of(directoryDelimiters.begin(), directoryDelimiters.end(), [&subdirectory](char delim) {return subdirectory.find(delim) != std::string::npos; })){
                std::cout << "Invalid subdirectory name. Please avoid using '/', '\\', '..', or ':'." << std::endl;
            }
            break;
        }
        return subdirectory;
    }

    // TODO: According to Claude 3.7 this function uses the subdirectory parameter ineffectively, fix it.
    FsPath FileHandler::GetUserDirectory(std::string_view subdirectory) const{
        //@NOTE (ari): TODO
        FsPath directory{};

        try{
            #ifdef _WIN32 // WINDOWS
            const char* homeDir = std::getenv("USERPROFILE");
            if(!userProfile){
                throw std::runtime_error("Failed to resolve user profile directory!");
            }
            directory = homeDir;
            #else // LINUX/macOS
            const char* homeDir = std::getenv("HOME");
            if(!homeDir){
                throw std::runtime_error("Failed to resolve user profile directory!");      
            }
            directory = homeDir;
            #endif

            FsPath targetDir = directory / subdirectory;

            if(!std::filesystem::exists(targetDir)){
                try{
                    std::filesystem::create_directory(targetDir);
                    std::cout << "Created directory: " << targetDir << std::endl;
                } catch (const std::filesystem::filesystem_error& e){
                    std::cerr << "Failed to create directory " << e.what() << '\n';
                    std::cout << "Going back to default directory: 'Documents'... " << std::endl;

                    FsPath docsDir = directory / "Documents";
                    if(std::filesystem::exists(docsDir)){
                        return docsDir;
                        std::cout << "Default directory set: 'Documents'" << std::endl;
                    }
                    return directory;
                }
            }
            return targetDir;
        }
        catch(const std::exception& e){
            throw std::runtime_error(e.what());
        }  
    }

    bool FileHandler::CreateFile(const std::string& fileName){
        
        CloseFile();

        const std::string userSubdirectory = GetSubdirectory();
        FsPath subdirectory = GetUserDirectory(userSubdirectory);

        try{
            if(std::filesystem::exists(fileName)){
                throw std::runtime_error("File already exists!");
            }

            #ifdef _WIN32
            const FsPath fullPath = subdirectory / fileName;
            #else
            const FsPath fullPath = subdirectory / fileName;
            #endif

            std::ofstream newFile(fullPath);

            if(!newFile.is_open()){
                throw std::runtime_error("Failed to create file: " + fullPath.string());
            }

            newFile.close();
            
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
            return false;
        }
    }
} //namespace FileHandler