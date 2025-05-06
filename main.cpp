#include "src/App.h"

#include <cassert>

 int main(){
    Fitter::Fitter App;

    App.Start();

    App.DisplayMenu();

    while(App.IsAppRunning()){
        App.Update();
        
    }

    return 0;
 }
