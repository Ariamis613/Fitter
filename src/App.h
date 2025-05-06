#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

// forward declaration
namespace FileHandler{
  class FileHandler;
}

namespace Fitter{

class Fitter{
public:
  Fitter(const std::string name, const int sets, const int reps, const float weight);
  Fitter();
  ~Fitter();
  
  //@squeeze: Getters
  std::string GetExcersiseName() const {return m_name;}
  int GetSets() const {return m_sets;}
  int GetReps() const {return m_reps;}
  int GetWeightKG() const {return m_weight_kg;}
  bool IsAppRunning() {return isRunning;}
  int getWeightLBS() const {return m_weight_lbs;}
  std::vector<Fitter> LogExercise(const Fitter& exercise);

  void Start();
  void Delete(); // TODO: Implement
  void PrintExerciseObject(const Fitter& ex);
  void PrintExerciseVector(const std::vector<Fitter>& exercises);
  static void ClearScreen();
  void Update();
  Fitter DisplayMenu();
  friend std::ostream& operator<<(std::ostream& os, const Fitter& obj);

  bool isRunning = true;
  uint8_t choice = 0;

private:
  std::unique_ptr<FileHandler::FileHandler> FileHandler_ptr;
  std::string m_name;
  int m_sets{0};
  int m_reps{0};
  float m_weight_kg{0.00f};
  float m_weight_lbs{0};
  std::vector<Fitter> exerciseBuffer{};
};
} //namespace Fitter