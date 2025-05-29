FROM gcc:latest

# Install CMake
RUN apt-get update && apt-get install -y cmake && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source files and CMake configuration
COPY . .

# Build using CMake
RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build .

# Set the working directory to where the binary is located
WORKDIR /app/build/bin

# Run the application
CMD ["./Fitter"]