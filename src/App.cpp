#include "App.h"
#include "FileHandler.h"
#include "Utils.h"
#include "database/Database.h"

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
  
  Fitter::Fitter() {
  }

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
    std::printf("Welcome to Fitter! \n");
    std::printf("Your personal workout tracking application\n");
    std::printf("Choose an option: \n");
    std::printf("1. Log an exercise\n");
    std::printf("2. Access the file manager\n");
    std::printf("----------------------------------------\n");

    // Initialize resources
    exerciseBuffer.clear();
    pFileHandler = std::make_shared<FileHandler::FileHandler>();
    
    isRunning = true;
  }

  Fitter Fitter::TakeInput() const{
    std::string name = "";
    unsigned int reps{0};
    unsigned int sets{0};
    float weight{0.00f};
    
    // Get exercise name with validation
    std::cout << "Enter exercise name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);
    
    // Validate name is not empty
    while(name.empty() || name.find_first_not_of(" \t\n\r") == std::string::npos) {
        std::cout << "Exercise name cannot be empty. Please enter a valid name: ";
        std::getline(std::cin, name);
    }

    // Get reps with validation
    std::cout << "Enter number of reps: ";
    while(!(std::cin >> reps) || reps == 0) {
        std::cout << "Please enter a valid number of reps (greater than 0): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Get sets with validation
    std::cout << "Enter number of sets: ";
    while(!(std::cin >> sets) || sets == 0) {
        std::cout << "Please enter a valid number of sets (greater than 0): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Get weight with validation
    std::cout << "Enter weight in kg: ";
    while(!(std::cin >> weight) || weight < 0) {
        std::cout << "Please enter a valid weight (0 or greater): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    Fitter exercise(name, sets, reps, weight, m_time);

    return exercise;
  }

  Fitter Fitter::DisplayMenu(Database::Database* db){
    
    Fitter exercise = TakeInput();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    FileHandler::FileHandler::ClearScreen();
    
    std::cout << "Save to database? (y/n): ";
    char choice;
    std::cin >> choice;

    if(choice == 'y' || choice == 'Y'){
      if(db && db->SaveToDatabase(exercise)){
        std::cout << "✓ Exercise saved to database successfully!\n";
      } else {
        std::cout << "✗ Failed to save exercise to database.\n";
      }
    } else if(choice == 'n' || choice == 'N') {
        std::cout << "Exercise not saved.\n";
    } else {
        std::cout << "Invalid choice. Exercise not saved.\n";
    }
    
    return exercise;
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

      const int fChoice = pFileHandler->GetChoice();
      
      // Exit early if user chose to exit
      // MAX_CHOICE would always be the exit choice
      if(fChoice == FileHandler::MAX_CHOICE){
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

      switch(fChoice){
        case 1:
          printf("Creating file: %s \n", fileName.c_str());
          success = pFileHandler->CreateFile(fileName) && pFileHandler->SaveToFile(this, fileName);
          if(success){
            std::printf("File %s created and saved successfully!\n", fileName.c_str());
          } else{
            std::cerr << "Failed to create file: " << fileName << std::endl;
          }
          break;

        // @NOTE: Merged with case 1
        // case 2:
        //   printf("Saving to file: %s \n", fileName.c_str()); 
        //   success = pFileHandler->SaveToFile(this, fileName);
        //   if(!success){
        //     std::cerr << "Failed to save to file: " << fileName << std::endl;
        //   }
        //   break;
        
        case 2:{
          printf("Reading file: %s \n", fileName.c_str());
          auto lines = pFileHandler->ReadFile(fileName);

          if(!lines.has_value() || lines->empty()){
            printf("No content found in file %s\n", fileName.c_str());
            printf("The file may be empty or could not be opened.\n");
          } else{
            std::cout << fileName << " contents (" << lines->size() << " lines): " << std::endl;
            for(const auto& line : *lines){
              std::cout << line << '\n';
            }
            printf("----------------------------------------\n");
          }
          break;
        }
        case 3:
          std::printf("Deleting file: %s\n", fileName.c_str());
          try{
            pFileHandler->DeleteFile(fileName.c_str());
            if(!std::filesystem::exists(fileName)){
              std::printf("File deletion operation completed.\n");
            }
            std::printf("----------------------------------------\n");
          } catch(const std::exception& e) {
            std::cerr << "Error during file deletion: " << e.what() << std::endl;
          }
          break;
        
        case 4:
          std::printf("Appending to file: %s\n", fileName.c_str());
          success = pFileHandler->AppendToFile(this, fileName);
          if(!success) {
            std::cerr << "Failed to append to file: " << fileName << std::endl;
          } else {
            std::cout << "Successfully appended to file: " << fileName << std::endl;
          }
          break;

        default:
          std::cerr << "Invalid fChoice!" << std::endl;
          break;
      }
      isRunning = false;
    }
  }
} // namespace Fitter
