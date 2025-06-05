#pragma once

#include <sqlite3.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Forward declaration
namespace Fitter {
    class Fitter;
}

namespace Database{
class Database{
public:
    Database(const std::string& dbPath);
    Database();
    ~Database();

    bool InitializeDatabase();
    bool SaveToDatabase(const Fitter::Fitter& exercise);
    json ToJson(const Fitter::Fitter& exercise) const;
    void CloseDatabase();

private:
    sqlite3* db = nullptr;
    std::string m_dbPath = "";
    bool BindParams(sqlite3_stmt* stmt, const Fitter::Fitter& exercise);
};

} // namespace Database