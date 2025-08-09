#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream> 
#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 
#include <vector> 
#include "Memory.h" 
#include "Cpu.h"
#include "Screen.h" 
#include "Keypad.h"
#include "DebuggingTextbox.h"

class Cpu; 
class Memory; 
class Screen; 
class Keypad; 
class DebuggingTextbox;

class Debugger {
    private: 
    bool debuggerIsOn; 
    SDL_Window* debuggingWindow;  
    SDL_Renderer* debuggingRenderer; 
    const char* fontRegular;
    const char* fontExtraBold; 
    const char* fontSemiBold;
    std::vector <std::string> stackPrintingVector; 
    

    public: 
    bool runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger debugger); 
    void setDebuggerIsOn(bool value); 
    bool getDebuggerIsOn(); 
    SDL_Renderer* getDebuggingRenderer(); 
    bool initializeDebugger(); 
    bool destroyDebuggerWindow(DebuggingTextbox debuggingTextbox); 
    void outputCurrentInstructionToDebugger(std::string instruction); 
    void outputRegistersToDebugger(uint8_t registerValue, int registerName); 
    std::string convertIntToString(int value, bool add0xPrefix, bool convertToHexDigits); 
    void outputStackToDebugger(Memory memory); 
    std::string convertIntToHexString(int value); 

    Debugger(); 
    ~Debugger(){}; 

}; 

#endif 