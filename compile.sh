#!/bin/bash

# Create the bin directory if it doesn't exist
mkdir -p bin

# Export the standard header path
export CPATH="/usr/include/c++/15:/usr/include"

# Compile with explicit path to libstdc++
g++ -std=c++23 main.cpp src/*.cpp -o bin/Fitter \
  -I/usr/include/c++/15 \
  -I/usr/include \
  -L/usr/lib64 \
  -lstdc++ \
  -lstdc++fs 