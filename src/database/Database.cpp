

#include <iostream>

#include "../App.h"
#include "Database.h"

namespace Database{
Database::Database(const std::string& dbPath) : m_dbPath(dbPath){}

Database::Database() = default;

Database::~Database(){
    CloseDatabase();
}

bool Database::InitializeDatabase(){
    int rc = sqlite3_open(m_dbPath.c_str(), &db);
    if(rc != SQLITE_OK){
        std::cerr << "Error opening database: " << sqlite3_errmsg(db)
        << std::endl;
        return false;
    }

    const char* sqlTable = R"(
        CREATE TABLE IF NOT EXISTS exercises (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        sets INTEGER NOT NULL,
        reps INTEGER NOT NULL,
        weight_kg REAL NOT NULL,
        weight_lbs REAL NOT NULL,
        timestamp INTEGER NOT NULL,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
      );
    )";

    char* errMsg = 0;
    rc = sqlite3_exec(db, sqlTable, 0, 0, &errMsg);

    if(rc != SQLITE_OK){
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool Database::BindParams(sqlite3_stmt* stmt, const Fitter::Fitter& exercise){
    if(!stmt){
        std::cerr << "Invalid statement pointer!" << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, exercise.GetExcersiseName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, exercise.GetSets());
    sqlite3_bind_int(stmt, 3, exercise.GetReps());
    sqlite3_bind_double(stmt, 4, exercise.GetWeightKG());
    sqlite3_bind_double(stmt, 5, exercise.GetWeightLBS());
    sqlite3_bind_int64(stmt, 6, static_cast<int64_t>(exercise.GetTimestamp()));

    return true;
}

bool Database::SaveToDatabase(const Fitter::Fitter& exercise){
    if(!db){
        std::cerr << "Database not initialized" << std::endl;
        return false;
    }

    const char* sql = R"(
        INSERT INTO
        exercises (name, sets, reps, weight_kg, weight_lbs, timestamp)
        VALUES
        (?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if(rc != SQLITE_OK){
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
        << std::endl;
        return false;
    }

    BindParams(stmt, exercise);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(rc != SQLITE_DONE){
        std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

json Database::ToJson(const Fitter::Fitter& exercise) const{
    return json{
        {"name", exercise.GetExcersiseName()},
        {"sets", exercise.GetSets()},
        {"reps", exercise.GetReps()},
        {"weight_kg", exercise.GetWeightKG()},
        {"weight_lbs", exercise.GetWeightLBS()},
        {"timestamp", exercise.GetTimestamp()}
    };
}

void Database::CloseDatabase(){
    if(db){
        sqlite3_close(db);
        db = nullptr;
    }
}
} // namespace Database