Pestofish
==============
Pestofish2 is a WIP improvement on the original [PestoFish](https://github.com/KirilBourakov/Pestofish), featuring an
engine written in C++.

# Code
main.py - Call the to run the game \
runtests.py - Runs tests for the engine. Currently not functional.
## Game
The folder contains the code for the game itself; the GUI, the SFX, the assets, the various states, etc.

## cEngine
PestoFish2's C++ engine, currently WIP.

## Engine
The original PestoFish Engine

## Builders 
Contains a set of builders for the engine. Builders are scripts to be run once to create information for the engine.
- network: scripts used to train and create, and pre-process data for the neural network
- open-db: scripts used to create the openings database. The source for our openings database was the wikipedia page that contains all openings and their moves.
- piece-square: scripts used to create piece square values; values that correspond with how good a position is for a piece depending on it's color, type and if it's a middle or end game. 
- min-test: a small script I used to find an error I was having when rewriting how the engine handled scaling across cores.

## Plans and Notes
Contains plans for the engine (made with draw.io), as well as notes for reaserch we did. Note these are not formatted appropriately. 

# Running
Pestofish uses python 3.12.3. Other versions of python should work, if they have access to Tensorflow 2, and pygame. 
The below steps, depending on the version of python, may not work, however. 

With python3 installed, run:
1. In the root repository directory, create a python virtual environment:
```python3 -m venv .venv```
2. Activate your environment.
3. Install dependencies:
```pip install -r requirements.txt```

This project has 3 top level dependencies: 
- pygame (for graphics)
- tensorflow (for machine learning) 
- mypy (for type checking; it is not needed to actually run the game)

While mypyc is included as a dependency, it is not used; the compiler is still in development, and seems to have problems with multiprocessing. It also lacks other python features, such as \_\_file\_\_.

# Attribution
JohnPablok's improved Cburnett chess set.

