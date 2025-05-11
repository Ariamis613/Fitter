#include "FileHandler.h"
#include "App.h"
#include "Utils.h"


#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <algorithm>

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

    bool FileHandler::SaveToFile(Fitter::Fitter* object, std::string_view fileName){
        if(object == nullptr || fileName.empty()){
            std::cerr << "Cannot initialize object or file! " << std::endl;
            return false;
        }
        // HERE
        // m_fileStream.

    }

    bool FileHandler::CreateFile(const std::string& fileName){
        CloseFile();

        const std::string userSubdirectory = utils::GetSubdirectory();
        FsPath subdirectory = utils::GetUserDirectory(userSubdirectory);

        try{
            if(std::filesystem::exists(fileName)){
                std::cerr << "File already exists!" << std::endl;
            }

            #ifdef _WIN32
            const FsPath fullPath = subdirectory / fileName;
            #else
            const FsPath fullPath = subdirectory / fileName;
            #endif

            std::ofstream newFile(fullPath);

            if(!newFile.is_open()){
                std::cerr << "Failed to create file: " + fullPath.string();
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

    bool FileHandler::IsValidFileName(const std::string& fileName) const{
        if(fileName.empty()){
            std::cerr << "File name cannot be empty!" << std::endl;
            return false;
        }

        static constexpr std::array<char, 9> invalidChars = {
            '\\', '/', ':', '*', '?', '\"', '<', '>', '|'
        };

        for(const auto& invalid : invalidChars){
            if(fileName.find(invalid) != std::string::npos){
                std::cerr << "File name contains invalid characters!" << std::endl;
                return false;
            }
        }

        if (fileName.front() == ' ' || fileName.back() == ' ' ||
        fileName.front() == '.' || fileName.back() == '.'){
            return false;
        }
        return true;
    }

    // Gets and sets the file name based on user input
    std::string FileHandler::SetFileName(){
        std::string fileName{};

        std::getline(std::cin, fileName);
        m_fileName = fileName;

        if(!IsValidFileName(m_fileName)){
            std::cerr << "Filename is invalid!" << std::endl;
            return "";
        }

        return m_fileName;
    }
} //namespace FileHandler