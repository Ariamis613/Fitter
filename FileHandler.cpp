#include "FileHandler.h"

#include <stdexcept>
#include <iostream>
#include <string>
#include <sys/stat.h>

#ifdef _WIN32
constexpr std::string_view WinPath = "C:\\Users\\$USER$\\Documents";
#else
constexpr std::string_view LinuxPath = "/home/$USER/Documents";
#endif

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

    bool FileHandler::ScanDirectoryForFile(std::string_view fileName, std::string_view directory){

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

    void FileHandler::CreateFile(const std::string& fileName){
        CloseFile();

        #ifdef _WIN32
        const std::string directory = "C:\\Users\\$USER$\\Documents";
        #else
        const std::string directory = "/home/$USER/Documents";
        #endif

        try{
            if(ScanDirectoryForFile(fileName, directory)){
                throw std::runtime_error("File already exists!");
            }

            #ifdef _WIN32
            const std::string fullPath = directory + "\\" + fileName;
            #else
            const std::string fullPath = directory + "/" + fileName;
            #endif

            std::ofstream newFile(fullPath);
            if(!newFile.is_open()){
                throw std::runtime_error("Newly created file is not open!");
            }

            newFile.close();
            
            printf("File created successfully: %s\n", fullPath.c_str());
        }
        catch(const std::exception& e){
            std::cout << e.what() << std::endl;
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