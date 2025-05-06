#pragma once

#include "nlohmann/json.hpp"
#include "FileHandler.h"

namespace JSON {

class JSON : public FileHandler::FileHandler{
public:

    JSON();

    JSON(const std::string file);

    JSON(const nlohmann::json& json);

    ~JSON();

    JSON& Parse() const{

    }

private:
    std::string m_file;
    nlohmann::json data;
};
} // namespace JSON