#include "src/App.h"

#include <cassert>
#include <iostream>

int main(){
    Fitter::Fitter App;

    App.Start();

    std::cin >> App.choice;

    if(App.choice == 1){
        std::cout << "=== Exercise Logging ===" << std::endl;
        Fitter::Fitter exerciseData = App.DisplayMenu();
        
        // Copy the exercise data to the main App object
        App = exerciseData;
        
        std::cout << "Exercise logged! Now accessing file manager..." << std::endl;
    } else if(App.choice == 2){
        // User chose to go directly to file manager
        std::cout << "=== File Manager ===" << std::endl;
    } else {
        std::cerr << "Invalid choice! Defaulting to file manager..." << std::endl;
    }

    while(App.isRunning){
        App.Update();
    }

    return 0;
}