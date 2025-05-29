#pragma once

#include <string>
#include <ctime>

namespace Goal{
class Goal{
public:
    Goal(std::string name, double targetWeight, uint targetReps, std::time_t deadline);

    ~Goal();

    void SetName(std::string_view name);
    void SetTargetWeight(double targetWeight);

private:
    std::string m_name;
    double m_targetWeight;
    uint m_targetReps;
    std::time_t m_deadline;
    bool m_achieved = false;
};
} // namespace Goal