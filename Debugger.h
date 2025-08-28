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

class Cpu; 
class Memory; 
class Screen; 
class Keypad; 

class Debugger {
    private: 
    bool debuggerIsOn; 
    SDL_Window* debuggingWindow;  
    SDL_Renderer* debuggingRenderer; 
    const char* fontRegular;
    const char* fontExtraBold; 
    const char* fontSemiBold;
    std::vector <std::string> stackPrintingVector; 
    std::vector <std::string> pastInstructionVector; 
    std::vector <std::string> pastProgramCounterVector; 

    const char* font; 
    const char* messageText; 
    int x; 
    int y; 
    int width;
    int height; 
    bool textIsStatic; 
    std::string textColor;

    TTF_Font* messageFont; 

    public: 
    bool runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger debugger); 
    void setDebuggerIsOn(bool value); 
    bool getDebuggerIsOn(); 
    SDL_Renderer* getDebuggingRenderer(); 
    bool initializeDebugger(); 
    bool destroyDebuggerWindow(); 
    void outputCurrentInstructionToDebugger(std::string instruction); 
    void outputRegistersToDebugger(uint8_t registerValue, int registerName); 
    void outputProgramCounterToDebugger(uint16_t programCounter); 
    std::string convertIntToString(int value, bool add0xPrefix, bool convertToHexDigits); 
    void outputStackToDebugger(Memory memory); 
    std::string convertIntToHexString(int value); 
    void outputDelayTimerToDebugger(uint8_t delayTimer); 
    void outputIndexRegisterToDebugger(std::string address); 
    void outputPastInstructionsToDebugger(std::string currentInstruction); 
    void outputPastPCToDebugger(std::string programCounter); 
    void createBoxAndAddText(const char* font, const char* text, int x, int y, int width, int height, bool textIsStatic, SDL_Renderer* DebuggingRenderer, 
        std::string textColor); 
    void resetDataOnDebuggerScreen(); 
    void resetRegisterData(); 
    void resetPCAndInstructionData(); 
    void resetStackData(); 
    void resetIndexRegisterData(); 
    void resetDelayAndSoundTimerData(); 

    Debugger(); 

    ~Debugger(){}; 

}; 

#endif 