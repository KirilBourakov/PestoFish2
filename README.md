Pestofish2
==============
<p align="center">
  <img src="frontend/assets/pesto.png" alt="Pestofish2 Logo">
</p>

Pestofish2 is an open source chess engine written in C++. It is a significant improvement over the original [PestoFish](https://github.com/KirilBourakov/Pestofish), written as part of a first year final project.

## Table of Contents
*   [Features](#features)
*   [Project Structure](#project-structure)
*   [Quickstart](#quickstart)
    *   [Requirements](#requirements)
    *   [Build Instructions (Engine)](#build-instructions-engine)
    *   [Usage Instructions (Evaluate)](#usage-instructions-evaluate)
    *   [Usage Instructions (Frontend)](#usage-instructions-frontend)
*   [Attribution](#attribution)

## Features
*   **Advanced Search**: 
    - Negamax with Alpha-Beta pruning
    - Iterative Deepening & Aspiration windows
    - Quasiance Search
    - LazySMP for efficent multithreading
    - Principle Variation Search (PVS) 
    - Late Move Reductions
    - Check Extensions
*   **Advanced Evluation**:
    - Efficently Updateable Neural Network (NNUE) with 768 weight accumulator and a 640 weight hidden layer.
*   **Efficent Move Ordering**:
    - Best Historical Move
    - MVV-LVA sorted captures
    - Killer Quite Moves
    - History Based Quite Move 
*   **UCI Compatible**:
    - Supports core UCI features, allowing use with popular chess GUIs (CuteChess, etc.).
*   **High Performance**: Written in modern C++20 with efficient bitboard-based move generation.


## Project Structure

*   **`pestofish`**: The C++ source code for the chess engine.
*   **`frontend`**: A Python-based GUI (using PyQt6) to play against the engine.
*   **`evaluate`**: Scripts used to compare and evaluate different versions of the engine.

# Quickstart

## Requirements

*   **C++ Compiler**: C++20 compliant compiler (GCC, Clang, MSVC).
*   **CMake**: Version 3.27 or higher.
*   **Python**: Version 3.10 or higher (for the frontend).

## Build Instructions (Engine)

The engine core is located in the `pestofish` directory. To build it:

1.  Navigate to the `pestofish` directory:
    ```bash
    cd pestofish
    ```

2.  Configure the build with CMake:
    ```bash
    cmake -S . -B build
    ```

3.  Build the engine (Release mode recommended for performance):
    ```bash
    cmake --build build --config Release
    ```

## Usage Instructions (Evaluate)

1.  Place two versions of Pestofish (or any other UCI engine) into baseline/ and new/

2. Run the ``test.bat`` script.

## Attribution
*   [lczero Opening Books](https://github.com/jhorthos/lczero-training/wiki/Opening-Books) - Used for evaluating new versions of pestofish.
*   [Leorik (v3.1)](https://github.com/lithander/Leorik) - NNUE weights used by the engine.