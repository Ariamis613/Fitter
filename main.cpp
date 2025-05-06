#include "src/App.h"

#include <cassert>
#include <memory>

 int main(){
    Fitter::Fitter App;

    std::unique_ptr<FileHandler::FileHandler> FileHandler = 
    std::make_unique<FileHandler::FileHandler>();

    App.Start();

    App.DisplayMenu();

    while(App.IsAppRunning()){
        App.Update();
        
    }

    return 0;
 }
