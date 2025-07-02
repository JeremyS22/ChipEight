#include <iostream> 
#include "Cpu.h" 
#include "Cpu.cpp" 
#include "Screen.h"
#include "Screen.cpp" 
#include "Memory.h" 
#include "Memory.cpp" 

#include "src/include/SDL2/SDL.h" 
#include <chrono> 
#include <thread> 

using namespace std; 

int main (int argv, char** args){

    Screen screen;  
    Memory memory; 
    
    int instructionsPerSecond = 700; 
    
    bool windowIsRunning = true; 
    SDL_Event windowEvent; 

    string romFileLocation = "ROMS/IBM Logo.ch8"; 
    

    screen.initializeScreen(); 

    memory.loadFontDataIntoMemory(fontData, memory); 
    
    cout << "PROGRAM COUNTER after loading font data in Memory " << getProgramCounter() << endl; 

    memory.loadRomIntoMemory(memory, romFileLocation, &programCounter); 

    cout << "PROGRAM COUNTER after loading roms in Memory " << getProgramCounter() << endl; 

    while(windowIsRunning){
        if (SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break; 
            }

            for(int instructionCounter = 0; instructionCounter < instructionsPerSecond; instructionCounter++){
                fetchInstructions(memory); 
                decodeAndExecuteInstructions(currentInstruction, screen, memory); 
            }
            this_thread::sleep_for(chrono::seconds(1)); 
        }
    }   

    screen.destroyCreatedWindow(); 

return 0; 

}