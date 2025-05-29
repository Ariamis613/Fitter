#include "App.h"
#include "FileHandler.h"
#include "Utils.h"

extern "C"{
  #include <assert.h>
}

#include <iostream>
#include <limits>

namespace Fitter{
  Fitter::Fitter(const std::string name, const unsigned int sets, const unsigned int reps, const float weight,
    std::time_t localTime) : m_name(name), m_sets(sets), m_reps(reps), m_weight_kg(weight), m_time(localTime){
    m_weight_lbs = Utils::ConvertToLbs(weight);
  }
  
  Fitter::Fitter() = default;

  Fitter::~Fitter(){
    // Clean up resources if necessary
    exerciseBuffer.clear(); // Clear the exercise buffer
    pFileHandler.reset(); // Reset the shared pointer to release the FileHandler object
  }

  // Copy assignment operator
  Fitter& Fitter::operator=(const Fitter& other) {
    if (this != &other) {
      m_name = other.m_name;
      m_sets = other.m_sets;
      m_reps = other.m_reps;
      m_weight_kg = other.m_weight_kg;
      m_weight_lbs = other.m_weight_lbs;
      m_time = other.m_time;
      // Don't copy pFileHandler or exerciseBuffer as they're implementation details
    }
    return *this;
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
                                     ", Time: " << obj.m_time;
    return os;
  }

  void Fitter::PrintExerciseObject(const Fitter& obj){
    Fitter instance(obj.m_name, obj.m_sets, obj.m_reps, obj.m_weight_kg, obj.m_time);
    std::cout << instance << std::endl;

    std::cout << "PrintExerciseObject called" << std::endl;
  }

  void Fitter::Start(){
    printf("Welcome to Fitter! \n");
    printf("Your personal workout tracking application\n");
    printf("----------------------------------------\n");
    
    // Initialize resources
    exerciseBuffer.clear();
    pFileHandler = std::make_shared<FileHandler::FileHandler>();
    
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

    return Fitter(name, sets, reps, weight, m_time);
  }

  void Fitter::Update(){
    while(isRunning){
      try{
        pFileHandler->DisplayChoice();
      } catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        isRunning = false;
        return;
      }

      const int choice = pFileHandler->GetChoice();
      
      // Exit early if user chose to exit
      if(choice == 6){
        printf("Exiting the program...\n");
        isRunning = false;
        break;
      }

      // Get filename - only needed for choices 1-5
      pFileHandler->SetFileName();
      const std::string fileName = pFileHandler->GetFileName();

      if(fileName.empty()){
        std::cerr << "No file name provided!" << std::endl;
        isRunning = false;
        return;
      }
      bool success = false;

      switch(choice){
        case 1:
          printf("Creating file: %s \n", fileName.c_str());
          success = pFileHandler->CreateFile(fileName);
          if(success){
            printf("File %s created successfully!\n", fileName.c_str());
          } else{
            std::cerr << "Failed to create file: " << fileName << std::endl;
          }
          break;
    
        case 2:
          printf("Saving to file: %s \n", fileName.c_str()); 
          success = pFileHandler->SaveToFile(this, fileName);
          if(!success){
            std::cerr << "Failed to save to file: " << fileName << std::endl;
          }
          break;
        
        case 3:{
          printf("Reading file: %s \n", fileName.c_str());
          auto lines = pFileHandler->ReadFile(fileName);

          if(!lines.has_value() || lines->empty()){
            printf("No content found in file %s\n", fileName.c_str());
            printf("The file may be empty or could not be opened.\n");
          } else{
            std::cout << fileName << " contents (" << lines->size() << " lines): " << std::endl;
            printf("----------------------------------------");
            for(const auto& line : *lines){
              std::cout << line << '\n';
            }
            printf("----------------------------------------\n");
          }
          break;
        }
        case 4:
          printf("Deleting file: %s\n", fileName.c_str());
          try {
            pFileHandler->DeleteFile(fileName.c_str());
            printf("----------------------------------------\n");
            printf("File deletion operation completed.\n");
          } catch(const std::exception& e) {
            std::cerr << "Error during file deletion: " << e.what() << std::endl;
          }
          break;
        
        case 5:
          printf("Appending to file: %s\n", fileName.c_str());
          success = pFileHandler->AppendToFile(this, fileName);
          if(!success) {
            std::cerr << "Failed to append to file: " << fileName << std::endl;
          } else {
            std::cout << "Successfully appended to file: " << fileName << std::endl;
          }
          break;

        default:
          std::cerr << "Invalid choice!" << std::endl;
          break;
      }
      isRunning = false;
    }
  }
} // namespace Fitter
