#include "src/App.h"

#include <cassert>

 int main(){
    Fitter::Fitter App;

    App.Start();

    // Store the exercise data returned from DisplayMenu
    Fitter::Fitter exerciseData = App.DisplayMenu();

    // Copy the exercise data to the main App object
    App = exerciseData;

    while(App.isRunning){
        App.Update();
    }

    return 0;
 }