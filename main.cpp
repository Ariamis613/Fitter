#include "src/App.h"
#include "src/database/Database.h"

#include <cassert>
#include <iostream>
#include <memory>

int main(){
    Fitter::Fitter App;
    std::unique_ptr<Database::Database> db = std::make_unique<Database::Database>("../fitter.db");
    
    if (!db->InitializeDatabase()) {
        std::cerr << "Failed to initialize database!" << std::endl;
        return 1;
    }

    App.Start();

    std::cin >> App.choice;

    if(App.choice == 1){
        std::cout << "=== Exercise Logging ===" << std::endl;
        Fitter::Fitter exerciseData = App.DisplayMenu();
        
        // Copy the exercise data to the main App object
        App = exerciseData;
        
        // Save to database
        if (db->SaveToDatabase(exerciseData)) {
            std::cout << "Exercise saved to database!" << std::endl;
        }
        
        std::cout << "Exercise logged! Now accessing file manager..." << std::endl;
    } else if(App.choice == 2){
        // User chose to go directly to file manager
        std::cout << "=== File Manager ===" << std::endl;
    } else {
        std::cerr << "Invalid choice! Defaulting to file manager..." << std::endl;
    }

    while(App.isRunning){
        App.Update();
        db->CloseDatabase();
    }

    return 0;
}