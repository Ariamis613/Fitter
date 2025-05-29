#include "Goal.h"

namespace Goal{
    Goal::Goal(std::string name, double targetWeight, uint targetReps, std::time_t deadline) 
    : m_name(name), m_targetWeight(targetWeight), m_targetReps(targetReps), m_deadline(deadline){}

    Goal::~Goal(){}

    void Goal::SetName(std::string_view name){
        m_name = name;
    }

    void Goal::SetTargetWeight(double targetWeight){
        m_targetWeight = targetWeight;
    }

} // namespace Goal