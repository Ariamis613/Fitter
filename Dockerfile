FROM gcc:latest

WORKDIR /app

# Copy all source files
COPY main.cpp .
COPY src/ ./src/

# Compile the application
RUN g++ main.cpp src/App.cpp src/FileHandler.cpp src/Utils.cpp -o main

CMD [ "./main" ]