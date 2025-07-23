#include <iostream> 

#include "Memory.h" 
#include "Cpu.h"
#include "Screen.h" 

class Cpu; 
class Memory; 
class Screen; 

class Debugger {
    public: 

    void runDebugger(SDL_Event windowEvent, Cpu cpu, Memory memory, Screen screen); 

}; 

