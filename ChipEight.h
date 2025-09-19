#ifndef CHIPEIGHT_H
#define CHIPEIGHT_H

#include <future> 
#include <chrono> 
#include <thread> 

#include "Cpu.h" 
#include "Screen.h"
#include "Memory.h" 
#include "Keypad.h" 
#include "Debugger.h"

class Cpu; 
class Screen; 
class Memory; 
class Memory; 
class Keypad; 
class Debugger;

class ChipEight {
    public:
        void initializeEmulator(Memory& memory, std::string romFileLocation, Cpu& cpu, Screen& screen, Debugger& debugger); 
        bool mainLoop(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger& debugger, bool inputToCloseEmulator, int instructionsPerSecond);
        void destroyEmulator(Debugger& debugger, Screen& screen, Cpu& cpu); 
        void waitForDelayTimerThreadToEnd(Cpu& cpu); 

        ChipEight(); 
        ~ChipEight(){}; 
}; 

#endif