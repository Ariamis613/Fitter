#pragma once
#include <string>
#include <vector>

namespace Fitter{

class Fitter{
  public:
  Fitter(const std::string name, const int sets, const int reps, const float weight);
  Fitter();
  ~Fitter();
  
  //@squeeze: Getters
  std::vector<Fitter> GetExerciseBuffer() const {return exerciseBuffer;}
  std::string GetExcersiseName() const {return m_name;}
  int GetSets() const {return m_sets;}
  int GetReps() const {return m_reps;}
  int GetWeightKG() const {return m_weight_kg;}
  bool IsAppRunning() const {return isRunning;}
  int getWeightLBS() const {return m_weight_lbs;}
  
  std::vector<Fitter> LogExercise(const Fitter& exercise);
  //@squeeze: TODO
  void Start();
  void PrintExerciseObject(const Fitter& ex);
  void PrintExerciseVector(const std::vector<Fitter>& exercises);
  void ClearScreen() const;
  void Update();
  //* returns user's input
  Fitter DisplayMenu();
  friend std::ostream& operator<<(std::ostream& os, const Fitter& obj);

  private:
  std::string m_name;
  int m_sets{0}; 
  int m_reps{0};
  float m_weight_kg{0.00f};
  int m_weight_lbs{0};
  bool isRunning = true;
  std::vector<Fitter> exerciseBuffer{};
};
} //namespace Fitter
