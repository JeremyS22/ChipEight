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
    
    int instructionsPerSecond = 700; 
    
    bool windowIsRunning = true; 
    SDL_Event windowEvent; 

    string romFileLocation = "ROMS/test_opcode.ch8"; 

    screen.initializeScreen(); 

    memory.loadFontDataIntoMemory(cpu, memory); 
    
    cout << "Status: (Main) PROGRAM COUNTER after loading font data in Memory " << cpu.getProgramCounter() << endl; 

    memory.loadRomIntoMemory(memory, romFileLocation, cpu); 

    cout << "Status: (Main) PROGRAM COUNTER after loading roms in Memory " << cpu.getProgramCounter() << endl; 

    memory.debug_printMemory(memory, true); 

    while(windowIsRunning){
        if (SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                windowIsRunning == false; 
                break; 
            }

            for(int instructionCounter = 0; instructionCounter < instructionsPerSecond; instructionCounter++){
                cpu.fetchInstructions(memory); 
                cpu.decodeAndExecuteInstructions(cpu.currentInstruction, screen, memory); 
            }
            this_thread::sleep_for(chrono::seconds(1)); 
        }
    }    

    screen.destroyCreatedWindow(); 

return 0; 

}
