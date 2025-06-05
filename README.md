# Fitter 🏋️

A lightweight, cross-platform workout tracking application built in modern C++ that helps you log, manage, and track your fitness exercises with persistent file storage.

## Features ✨

- **Exercise Logging**: Record exercise name, sets, reps, and weight (kg/lbs conversion)
- **File Management**: Create, save, read, append, and delete workout files
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Data Persistence**: Save your workout data in organized text files
- **User-Friendly Interface**: Clean menu-driven console interface
- **Input Validation**: Robust error handling and input validation
- **Flexible Storage**: Choose custom directories for your workout files
- **Modern C++**: Built with C++23/C++20 standards

## Quick Start 🚀

### Prerequisites

- **C++ Compiler**: 
  - GCC 11+ (for C++23) or GCC 10+ (for C++20)
  - Clang 14+ (for C++23) or Clang 12+ (for C++20)
  - MSVC 2022+ (for C++23) or MSVC 2019+ (for C++20)
- **CMake**: 3.20 or higher
- **Git**: For cloning the repository

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/Ariamis613/Fitter.git
   cd Fitter
   ```

2. **Build the project**
   
   **Option A: Using CMake (Recommended)**
   ```bash
   # Create build directory
   mkdir build && cd build
   
   # Configure the project
   cmake ..
   
   # Build the project
   cmake --build .
   
   # Optional: Install the binary
   cmake --install .
   ```
   
   **Option B: Using CMake with specific build type**
   ```bash
   # Debug build
   mkdir build-debug && cd build-debug
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   cmake --build .
   
   # Release build
   mkdir build-release && cd build-release
   cmake -DCMAKE_BUILD_TYPE=Release ..
   cmake --build .
   ```
   
   **Option C: Manual compilation (if CMake unavailable)**
   ```bash
   g++ -std=c++23 main.cpp src/App.cpp src/FileHandler.cpp src/Utils.cpp -o fitter
   ```
   
   **Option D: Using Docker**
   ```bash
   docker build -t fitter .
   docker run -it fitter
   ```

3. **Run the application**
   ```bash
   # If built with CMake
   ./bin/Fitter
   
   # If built manually
   ./fitter
   ```

## Usage 📖

### Main Menu Options

1. **Create a new file** - Initialize a new workout file
2. **Save to file** - Save current exercise data to an existing file
3. **Read from file** - View contents of a workout file
4. **Delete file** - Remove a workout file
5. **Append to file** - Add new exercise data to an existing file
6. **Exit program** - Close the application

### Example Workflow

1. Start the application and enter your exercise details
2. Choose option `1` to create a new workout file
3. Enter a filename (e.g., `monday_workout.txt`)
4. Choose option `2` to save your exercise data
5. Use option `3` to read and verify your saved data

### File Storage

- Files are stored in user-specified subdirectories
- Default fallback to `Documents` folder if directory creation fails
- Cross-platform path handling (Windows/Linux/macOS)

## Project Structure 📁

```
Fitter/
├── src/
│   ├── App.cpp          # Main application logic and menu system
│   ├── App.h            # Application class declarations
│   ├── FileHandler.cpp  # File I/O operations and management
│   ├── FileHandler.h    # File handling class declarations
│   ├── Utils.cpp        # Utility functions (conversions, directory handling)
│   └── Utils.h          # Utility function declarations
├── build/               # CMake build directory (created during build)
├── main.cpp             # Entry point
├── CMakeLists.txt       # CMake configuration file
├── Dockerfile           # Docker container configuration
└── README.md           # This file
```

## Technical Details 🔧

### Core Classes

- **`Fitter::Fitter`**: Main application class handling exercise data and workflow
- **`FileHandler::FileHandler`**: Manages all file operations (CRUD operations)
- **`Utils`**: Utility functions for weight conversion and directory management

### Key Features

- **Modern C++ Standards**: Built with C++23 (fallback to C++20)
- **Memory Management**: Smart pointers for automatic resource cleanup
- **Exception Handling**: Comprehensive error handling throughout the application
- **Cross-Platform Compatibility**: Conditional compilation for different operating systems
- **Input Validation**: Filename validation and sanitization
- **Weight Conversion**: Automatic kg ↔ lbs conversion

### File Format

Exercise data is stored in human-readable text format:
```
Exercise: Bench Press, Sets: 3, Reps: 10, Weight: 80kg, lbs: 176.37, Time: Mon Jan 15 14:30:25 2024
```

## Building from Source 🛠️

### CMake Build Options

```bash
# Standard build
cmake -B build
cmake --build build

# Debug build with verbose output
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --verbose

# Release build with optimizations
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Specify compiler (if multiple available)
cmake -B build -DCMAKE_CXX_COMPILER=g++-12
cmake --build build

# Cross-platform build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=your-toolchain.cmake
cmake --build build
```

### Manual Compilation (Alternative)

```bash
# C++23 build (recommended)
g++ -std=c++23 -Wall -Wextra -Wpedantic main.cpp src/*.cpp -o fitter

# C++20 build (fallback)
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp src/*.cpp -o fitter

# Debug build
g++ -std=c++23 -g -O0 -DDEBUG -Wall -Wextra main.cpp src/*.cpp -o fitter_debug

# Release build
g++ -std=c++23 -O3 -DNDEBUG -Wall -Wextra main.cpp src/*.cpp -o fitter
```

### Dependencies

- **Standard Library**: `<filesystem>`, `<fstream>`, `<iostream>`, `<memory>`, `<vector>`, `<string>`, `<chrono>`
- **System Libraries**: Platform-specific environment variable access
- **Build System**: CMake 3.20+
- **SQLite3**: For database operations
- **nlohmann/json**: For JSON parsing

## Contributing 🤝

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style

- Follow C++23/C++20 standards
- Use meaningful variable and function names
- Include comments for complex logic
- Maintain consistent indentation (spaces preferred)
- Use modern C++ features where appropriate

### Building for Development

```bash
# Setup development build
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build

# Run tests (when implemented)
cd build && ctest

# Format code (if clang-format available)
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## License 📄

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Roadmap 🗺️

- [ ] JSON file format support
- [ ] Exercise database with categories
- [ ] Progress tracking and analytics
- [ ] Workout session management
- [ ] Data visualization
- [ ] Unit testing framework
- [ ] Package managers support (vcpkg, Conan)

## Support 💬

If you encounter any issues or have questions:

1. Check the [Issues](https://github.com/Ariamis613/Fitter/issues) page
2. Create a new issue with detailed description
3. Include your operating system, compiler version, and CMake version

## Acknowledgments 🙏

- Built with modern C++23/C++20 features
- Cross-platform filesystem support
- CMake build system for easy compilation
- Inspired by the need for simple, effective workout tracking

---

**Happy lifting! 💪** 