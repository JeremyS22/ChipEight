#include <iostream> 

#include "src/include/SDL2/SDL.h" 
#include "Memory.h" 
#include "Cpu.h"
#include "Screen.h" 
#include "Keypad.h"

class Cpu; 
class Memory; 
class Screen; 
class Keypad; 

class Debugger {
    public: 

    void runDebugger(Cpu cpu, Memory memory, Screen& screen, Keypad keypad); 

    Debugger(); 
    ~Debugger(){}; 

}; 

