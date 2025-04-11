#include "FileHandler.h"

#include <stdexcept>
#include <iostream>
#include <string>

namespace FileHandler{
    
    explicit FileHandler::FileHandler(std::fstream& fileStream) : m_fileStream(fileStream) {}
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
    std::string FileHandler::ReadFile(const std::string& file){
        const std::ios_base::openmode mode = std::ios_base::in;

        std::string buffer;
        std::ifstream fileStream(file, mode);
        
    }

    bool FileHandler::FileExists(const std::string& fileName, const std::string_view directory){
        if(ScanDirectoryForFile(fileName, directory)){
            return true;
        }
        return false;
    }

    bool FileHandler::ScanDirectoryForFile(const std::string_view fileName, const std::string_view directory){
        std::filesystem::path dirPath(directory);
        if(!std::filesystem::is_directory(directory)){
            throw std::invalid_argument("Provided directory is invalid"); 
        }
        std::filesystem::path filePath(fileName);
        if(filePath.has_parent_path() || filePath.is_absolute()){
            throw std::invalid_argument("Invalid file name");
        }

        std::filesystem::path fullPath = dirPath / filePath;

        return std::filesystem::exists(fullPath) && std::filesystem::is_regular_file(fullPath);
    }

    void FileHandler::CreateFile(){
        CloseFile();


    }

    bool FileHandler::MoveFile(const std::string_view source, const std::string_view destination){
        try{
            std::filesystem::rename(source, destination);
            return true;
        }
        catch(const std::exception& e){
            std::cout << e.what() << std::endl;
            return false;
        }
    }
} //namespace FileHandler