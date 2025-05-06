#include "App.h"
#include "FileHandler.h"

extern "C"{
  #include <stdio.h>
  #include <assert.h>
}

#include <cstdlib>
#include <iostream>
#include <limits>

namespace Fitter{

  Fitter::Fitter(const std::string name, const int sets, const int reps, const float weight) : m_name(name), m_sets(sets), m_reps(reps), m_weight_kg(weight){

  }
  Fitter::Fitter() = default;
  Fitter::~Fitter(){
   ClearScreen();
  }

  void Fitter::PrintExerciseVector(const std::vector<Fitter>& exercises) {
    std::cout << "Exercise History (" << exercises.size() << " entries):" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    for(size_t i = 0; i < exercises.size(); i++) {
      std::cout << "Entry #" << (i+1) << ": ";
      PrintExerciseObject(exercises[i]);
    }
    
    std::cout << "----------------------------------------" << std::endl;
  }

  std::vector<Fitter> Fitter::LogExercise(const Fitter& exercise){
    try{
      exerciseBuffer.emplace_back(exercise);
      return exerciseBuffer;
    }
    catch(std::exception& e){
      std::cerr << e.what() << std::endl;
      return {};
    }
  }

  std::ostream& operator<<(std::ostream& os, const Fitter& obj) {
    os << "Fitter object output";  // Customize this based on actual Fitter data
    return os;
  }

  //@TODO:
  void Fitter::Start(){
    printf("Welcome to Fitter!");
    isRunning = true;
  }
  
  void Fitter::ClearScreen(){
    #ifdef _WIN32
      system("cls");
    #else
      system("clear");
    #endif
  }

  Fitter Fitter::DisplayMenu(){

    std::string name = "";
    int reps{0};
    int sets{0};
    float weight{0.00f};

    printf("Enter exercise name: ");
    std::getline(std::cin, name);

    printf("Enter number of reps: ");
    scanf("%d", &reps);

    printf("Enter number of sets: ");
    scanf("%d", &sets);

    printf("Enter weight in kg: ");
    scanf("%f", &weight);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return Fitter(name, reps, sets, weight);

    ClearScreen();
  }

  // HERE
  void Fitter::Update(){
    while(IsAppRunning()){
      FileHandler_ptr = std::make_unique<FileHandler::FileHandler>();
      
      if(FileHandler_ptr->GetChoice() == 1)


    }
  }

  void Fitter::Delete(){

  }

}//namespace Fitter
