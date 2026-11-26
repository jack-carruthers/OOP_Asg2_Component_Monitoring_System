# Aircraft Component Monitoring System 

A console application that can monitor aircraft components and report its status, it can be Normal, Caution, Maintenance Required or Non Operational

## Structure 
- `src/Status.h` - the Status values and helper functions
- `src/Component.h/.cpp` - base class with shared data 
- `src/EngineComponent.h/.cpp` - engine: temperature and vibration 
- `src/HydraulicPump.h/.cpp` - hydraulic pump: pressure and fluid level 
- `src/ComponentMonitor.h/.cpp` - owns the components and builds the list
- `src/InputParser.h` - converts typed text into numbers
- `src/main.cpp` - console menu
- `tests/tests.cpp` - automated tests (78 cases)

## Requirements 

C++17 complier (I used g++ from MinGW-w64/WinLibs).

## Build and run application 

From the project folder open a terminal and run:

  g++ -std=c++17 -Wall -Wextra -o acms src/Component.cpp src/EngineComponent.cpp src/HydraulicPump.cpp src/ComponentMonitor.cpp src/main.cpp
  ./acms.exe

## Build and run automated tests 

  g++ -std=c++17 -Wall -Wextra -o run_tests tests/tests.cpp src/Component.cpp src/EngineComponent.cpp src/HydraulicPump.cpp src/ComponentMonitor.cpp
  ./run_tests.exe

# Do note that for both the tests and application the exe will be left in repo so only the second line should needed be to typed into terminal to run it, for example just type ./acms.exe in terminal open to project folder to run application

