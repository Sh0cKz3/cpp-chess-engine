# C++ Chess Engine

An ongoing chess application and engine project written from scratch (high-level) in C++ using SFML.

![Chess board](assets/chess-board.png)

## Current Progress

- CMake-based C++ project setup
- Graphical chess board rendered using SFML
- Mouse-based board-square selection
- Object-oriented board and piece representation
- Initial chess position generation
- Separation of interface and implementation using header/source files

## Goals

- Implement legal chess movement
- Build a fully playable chess application
- Implement a chess engine
- Experiment with search algorithms and performance optimisation

## Technologies

- C++
- SFML
- CMake
- Git / GitHub

## Building the project

### Requirements

- C++ comiler
- CMake
- SFML 3

### Build

Clone the repository using:

    git clone https://github.com/Sh0cKz3/cpp-chess-engine.git
    cd cpp-chess-engine

Configure and build using CMake commands in terminal:

    cmake --preset default
    cmake --build build

Then run using the exectuable by standard command:

    ./build/ChessEngine.exe

## Project Structure

- `include/` — header files
- `src/` — source files
- `assets/` — graphical assets
- `docs/` — project documentation
- `CMakeLists.txt` — CMake project configuration
- `CMakePresets.json` — build configuration

## Status

This project is actively being developed.