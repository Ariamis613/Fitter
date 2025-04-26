#pragma once

#include "nlohmann/json.hpp"
#include "FileHandler.h"

namespace JSON {

class JSON : public FileHandler::FileHandler{
public:
    JSON();
    ~JSON();

private:
    nlohmann::json data;
};
} // namespace JSON