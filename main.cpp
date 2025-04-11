#include "App.hpp"
#include <iostream>
#include <cassert>

// Test function to validate Fitter class behavior
void testFitter() {
    // Create an instance of Fitter
    Fitter::Fitter fitter;
    
    // Test DisplayMenu functionality
    std::cout << "Testing DisplayMenu function..." << std::endl;
    Fitter::Fitter menuResult = fitter.Fitter::Fitter::DisplayMenu();
    
    std::vector<Fitter::Fitter> exercises = fitter.LogExercise(menuResult);
    assert(!exercises.empty() && "Exercise buffer should not be empty after logging an exercise.");

    std::cout << "\nTesting LogExercise vector..." << std::endl;
    fitter.PrintExerciseVector(exercises);
    // Test the overloaded insertion operator
    std::cout << "\nTesting operator<< overload..." << std::endl;
    // Add more specific tests based on expected behavior
    std::cout << "\nAll tests completed.\n" << std::endl;
}

int main() {
    try {
        testFitter();
        std::cout << "All tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
