#include <iostream> 
#include "Cpu.h" 
#include "Screen.h"
#include "Memory.h" 
#include "Keypad.h" 
#include "Debugger.h"
#include "ChipEight.h"

#include "src/include/SDL2/SDL.h" 
#include <chrono> 
#include <thread> 

using namespace std; 

int main (int argv, char** args){

    Screen screen;  
    Memory memory; 
    Debugger debugger; 
    Cpu cpu(debugger); 
    Keypad keypad; 
    ChipEight chipEight; 

    int instructionsPerSecond = 540; 
    debugger.setDebuggerIsOn(false); 
    string romFileLocation = "ROMS/6-keypad.ch8"; 
    bool inputToCloseEmulator = false; 
    
    chipEight.initializeEmulator(memory, romFileLocation, cpu, screen, debugger);     
    
    if(chipEight.mainLoop(cpu, memory, screen, keypad, debugger, inputToCloseEmulator, instructionsPerSecond) == true){
        return 0; 
    }
    
    chipEight.destroyEmulator(debugger, screen); 

return 0; 

}
