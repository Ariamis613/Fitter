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
    pFileHandler.reset(); // Reset the shared pointer to release the FileHandler object
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
       << ", Weight: " << obj.m_weight_kg << "kg, lbs: " << Utils::ConvertToLbs(obj.m_weight_kg) <<
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

    std::cout << "Enter exercise name: ";
    std::getline(std::cin, name);

    std::cout << "Enter number of reps: ";
    std::cin >> reps;

    std::cout << "Enter number of sets: ";
    std::cin >> sets;

    std::cout << "Enter weight in kg: ";
    std::cin >> weight;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    FileHandler::FileHandler::ClearScreen();

    return Fitter(name, reps, sets, weight);
  }

  // HERE
  void Fitter::Update(){
    while(isRunning){
      pFileHandler = std::make_shared<FileHandler::FileHandler>();

      try{
        pFileHandler->DisplayChoice();
      } catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        isRunning = false;
        return;
      }

      // Get filename
      pFileHandler->SetFileName();
      const std::string fileName = pFileHandler->GetFileName();

      if(fileName.empty()){
        std::cerr << "No file name provided!" << std::endl;
        return;
      }

      const int choice = pFileHandler->GetChoice();
      bool success = false;

      switch(choice){
        case 1:
          std::cout << "Creating file: " << fileName << std::endl;
          success = pFileHandler->CreateFile(fileName);
          if(success){
            std::cout << "File " << fileName << " created successfully!" << std::endl;
          }
          break;
    
        case 2:
          success = pFileHandler->SaveToFile(this, fileName);
          if(success){
            std::cout << "File " << fileName << " saved successfully!" << std::endl;
          }
          break;
        
        case 3:{
          auto lines = pFileHandler->ReadFile(fileName);
          if(lines.empty()){
            std::cout << "File " << fileName << " is empty!" << std::endl;
          } else {
            std::cout << fileName << " contents: " << std::endl;
            for(const auto& line : lines){
              std::cout << line << '\n';
            }
          }
          break;
        }

        default:
          std::cerr << "Invalid choice!" << std::endl;
          break;
      }
      isRunning = false;
    }
  }

  // void Fitter::DeleteFile(){

  // }

}//namespace Fitter
