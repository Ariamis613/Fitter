#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <ctime>
#include <chrono>

// forward declaration
namespace FileHandler{
  class FileHandler;
}

namespace Fitter{
class Fitter{
public:
  Fitter(const std::string name, const unsigned int sets,
        const unsigned int reps, const float weight, std::time_t localTime);
  Fitter();
  ~Fitter();
  
  // Copy assignment operator
  Fitter& operator=(const Fitter& other);
  
  std::string GetExcersiseName() const {return m_name;}
  int GetSets() const {return m_sets;}
  int GetReps() const {return m_reps;}
  float GetWeightKG() const {return m_weight_kg;}
  float GetWeightLBS() const {return m_weight_lbs;}
  std::tm* GetNow() const {return std::localtime(&m_time);}

  void Start();
  void PrintExerciseObject(const Fitter& ex);
  void Update();
  Fitter DisplayMenu();
  friend std::ostream& operator<<(std::ostream& os, const Fitter& obj);

  bool isRunning = true;
  int16_t choice = 0;

private:
  std::shared_ptr<FileHandler::FileHandler> pFileHandler = nullptr;
  std::time_t m_time = std::time(0);
  std::string m_name = "";
  int m_sets{0};
  int m_reps{0};
  float m_weight_kg{0.00f};
  float m_weight_lbs{0.00f};
  std::vector<Fitter> exerciseBuffer{};
};
} //namespace Fitter