#include <iostream> 
#include "Cpu.h" 
#include "Screen.h"
#include "Memory.h" 
#include "Cpu.h"

#include "src/include/SDL2/SDL.h" 
#include <chrono> 
#include <thread> 

using namespace std; 

int main (int argv, char** args){

    Screen screen;  
    Memory memory; 
    Cpu cpu; 
    
    int instructionsPerSecond = 540; 
    
    bool windowIsRunning = true; 
    SDL_Event windowEvent; 

    string romFileLocation = "ROMS/3-corax+.ch8"; 

    screen.initializeScreen(); 

    memory.loadFontDataIntoMemory(cpu, memory); 

    memory.loadRomIntoMemory(memory, romFileLocation, cpu); 

    memory.debug_printMemory(memory, true); 

    while(windowIsRunning){
        if (SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                windowIsRunning = false; 
                break; 
            }

            for(int instructionCounter = 0; instructionCounter < instructionsPerSecond; instructionCounter++){
                cpu.fetchInstructions(memory); 
                cpu.decodeAndExecuteInstructions(cpu.currentInstruction, screen, memory, cpu); 
            }
            this_thread::sleep_for(chrono::seconds(1)); 
        }
    }    

    screen.destroyCreatedWindow(); 

return 0; 

}
