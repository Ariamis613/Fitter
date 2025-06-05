cmake_minimum_required(VERSION 3.20)

# Project configuration
project(Fitter 
    VERSION 1.0.0
    DESCRIPTION "A lightweight, cross-platform workout tracking application"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fallback to C++20 if C++23 is not available
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|MSVC")
    set(CMAKE_CXX_STANDARD 20)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC and Clang options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # MSVC options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
endif()

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# Try to find nlohmann/json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    # Fallback: try to find it as a system package
    find_path(NLOHMANN_JSON_INCLUDE_DIR nlohmann/json.hpp
        PATHS /usr/include /usr/local/include /opt/homebrew/include
    )
    if(NLOHMANN_JSON_INCLUDE_DIR)
        message(STATUS "Found nlohmann/json headers at: ${NLOHMANN_JSON_INCLUDE_DIR}")
        set(NLOHMANN_JSON_FOUND TRUE)
    else()
        message(WARNING "nlohmann/json not found. Please install it or use a package manager.")
    endif()
endif()

# Include directories
include_directories(src)
if(NLOHMANN_JSON_INCLUDE_DIR)
    include_directories(${NLOHMANN_JSON_INCLUDE_DIR})
endif()

# Source files
set(SOURCES
    main.cpp
    src/App.cpp
    src/FileHandler.cpp
    src/Utils.cpp
    src/database/Database.cpp
)

# Header files (for IDE support)
set(HEADERS
    src/App.h
    src/FileHandler.h
    src/Utils.h
    src/database/Database.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SQLITE3_LIBRARIES})

# Link nlohmann/json if found
if(nlohmann_json_FOUND)
    target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
endif()

# Link filesystem library for older compilers
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

# Add SQLite3 include directories and compile flags
target_include_directories(${PROJECT_NAME} PRIVATE ${SQLITE3_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${SQLITE3_CFLAGS_OTHER})

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "SQLite3 found: ${SQLITE3_FOUND}")
message(STATUS "SQLite3 libraries: ${SQLITE3_LIBRARIES}")
if(nlohmann_json_FOUND)
    message(STATUS "nlohmann/json found: YES")
else()
    message(STATUS "nlohmann/json found: NO (using system headers)") 