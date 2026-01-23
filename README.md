Pestofish2
==============
Pestofish2 is an open source chess engine written in C++. It is a significant improvement over the original [PestoFish](https://github.com/KirilBourakov/Pestofish), written as part of a first year final project.

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

## Usage Instructions (Frontend)

The project includes a Python-based GUI located in the `frontend` directory.

1.  Navigate to the project root directory.

2.  Install the required Python dependencies:
    ```bash
    pip install -r requirements.txt
    ```

3.  Place a compiled version of pestofish into the engine directory.

4.  Run the frontend:
    ```bash
    python frontend/main.py
    ```

## Attribution
*   [lczero Opening Books](https://github.com/jhorthos/lczero-training/wiki/Opening-Books) - Used for evaluating new versions of pestofish.
*   [Leorik (v3.1)](https://github.com/lithander/Leorik) - NNUE weights used by the engine.