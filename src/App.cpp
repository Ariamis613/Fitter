#include "App.h"
#include "FileHandler.h"
#include "Utils.h"

extern "C"{
  #include <stdio.h>
  #include <assert.h>
}

#include <iostream>
#include <limits>

namespace Fitter{

  Fitter::Fitter(const std::string name, const unsigned int sets, const unsigned int reps, const float weight,
    std::time_t time) : m_name(name), m_sets(sets), m_reps(reps), m_weight_kg(weight), m_time(time){
  }
  Fitter::Fitter() = default;
  Fitter::~Fitter(){
    // Clean up resources if necessary
    exerciseBuffer.clear(); // Clear the exercise buffer
    fHandler.reset(); // Reset the shared pointer to release the FileHandler object
  }

  // void Fitter::PrintExerciseVector(const std::vector<Fitter>& exercises) {
  //   std::cout << "Exercise History (" << exercises.size() << " entries):" << std::endl;
  //   std::cout << "----------------------------------------" << std::endl;
    
  //   for(size_t i = 0; i < exercises.size(); i++) {
  //     std::cout << "Entry #" << (i+1) << ": ";
  //     PrintExerciseObject(exercises[i]);
  //   }
    
  //   std::cout << "----------------------------------------" << std::endl;
  // }

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
    os << "Name: " << obj.m_name << ", Sets: " << obj.m_sets << ", Reps: " << obj.m_reps
       << ", Weight: " << obj.m_weight_kg << "kg, lbs: " << utils::ConvertToLbs(obj.m_weight_kg) <<
                                     ", Time: " << std::asctime(std::localtime(&obj.m_time));
    return os;
  }

  //@TODO:
  void Fitter::Start(){
    printf("Welcome to Fitter! \n");
    isRunning = true;
  }

  Fitter Fitter::DisplayMenu(){

    std::string name = "";
    unsigned int reps{0};
    unsigned int sets{0};
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
    
    FileHandler::FileHandler::ClearScreen();

    return Fitter(name, reps, sets, weight);
  }

  // HERE
  void Fitter::Update(){
    while(isRunning){
      fHandler = std::make_shared<FileHandler::FileHandler>();

      fHandler->DisplayChoice();
      fHandler->SetFileName();

      const std::string fileName = fHandler->GetFileName();
      
      if(fHandler->GetChoice() == 1){
        printf("Creating file: %s\n", fileName.c_str());
        fHandler->CreateFile(fileName);
        printf("File %s created successfully!\n", fileName.c_str());
      }
      if(fHandler->GetChoice() == 2){
        assert(std::filesystem::exists(fileName));
        fHandler->SaveToFile(this, fileName);
        printf("File %s saved successfully!\n", fileName.c_str());
      }
      if(fHandler->GetChoice() == 3){
        for(const auto& line : fHandler->ReadFile(fileName)){
          std::cout << line << '\n';
        }
      }
      isRunning = false;
      return;
    }
  }

  // void Fitter::DeleteFile(){

  // }

}//namespace Fitter
