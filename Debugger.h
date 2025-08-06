#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream> 
#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 
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
    TTF_Font* messageFont; 
    const char* fontRegular;
    const char* fontExtraBold; 
    const char* fontSemiBold;
    

    public: 
    bool runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger debugger); 
    void setDebuggerIsOn(bool value); 
    bool getDebuggerIsOn(); 
    SDL_Renderer* getDebuggingRenderer(); 
    bool initializeDebugger(); 
    bool destroyDebuggerWindow(); 
    void createBoxAndAddText(const char* font, const char* messageText, int boxPositionX, int boxPositionY, int boxSizeWidth, int boxSizeHeight, bool textIsStatic); 
    void outputCurrentInstructionToDebugger(std::string instruction); 
    void outputRegistersToDebugger(uint8_t registerValue, int registerName); 
    const char* convertIntToCharPointer(int value); 
    
    Debugger(); 
    ~Debugger(){}; 

}; 

#endif 