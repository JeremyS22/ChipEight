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
    SDL_Window* debuggingWindow;  
    SDL_Renderer* debuggingRenderer; 
    TTF_Font* messageFont; 
    const char* fontRegular = "./assets/Inter_28pt-Regular.ttf"; 
    const char* fontExtraBold = "./assets/Inter_28pt-ExtraBold.ttf"; 
    const char* fontSemiBold = "./assets/Inter_28pt-SemiBold.ttf"; 
    SDL_Renderer* currentInstructionRenderer = SDL_CreateRenderer(debuggingWindow, 0, 0); 
    

    public: 
    bool runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger debugger); 
    void setDebuggerIsOn(bool value); 
    bool getDebuggerIsOn(); 
    bool initializeDebugger(); 
    bool destroyDebuggerWindow(); 
    void createBoxAndAddText(const char* font, const char* messageText, int boxPositionX, int boxPositionY, int boxSizeWidth, int boxSizeHeight); 
    void outputCurrentInstructionToDebugger(std::string instruction); 

    Debugger(); 
    ~Debugger(){}; 

}; 

#endif 