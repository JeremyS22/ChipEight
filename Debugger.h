#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream> 

#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 
#include "Memory.h" 
#include "Cpu.h"
#include "Screen.h" 
#include "Keypad.h"

class Cpu; 
class Memory; 
class Screen; 
class Keypad; 

class Debugger {
    private: 
    bool debuggerIsOn; 
    
    public: 
    bool runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad); 
    void setDebuggerIsOn(bool value); 
    bool getDebuggerIsOn(); 
    bool initializeDebugger(); 

    Debugger(); 
    ~Debugger(){}; 

}; 

#endif 