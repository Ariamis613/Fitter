//  #pragma once

//  #include "nlohmann/json.hpp"
//  #include "FileHandler.h"


//  namespace JSON {

//  class JSON : public FileHandler::FileHandler{
//  public:
//     JSON();

//     JSON(const std::string_view file);

//     JSON(const nlohmann::json& json);

//     ~JSON();

//     bool SaveToFile(Fitter::Fitter* object, FsPath file) override;
//     std::vector<std::string> ReadFile(const std::string& file) override;
//     bool CreateFile(const std::string& file) override;

//     // TODO: Implement later
//     JSON& Parse() const;

//  private:
//     std::string m_file;
//     nlohmann::json data;
//  };
//  } // namespace JSON