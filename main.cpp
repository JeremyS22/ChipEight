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
                        cout << "Clicked closed, EXITING " << endl; 
                        return 0; 
            }
            else if (windowEvent.type == SDL_KEYDOWN){
                   switch (windowEvent.key.keysym.sym){
                        case SDLK_q: case SDLK_ESCAPE: 
                            windowIsRunning = false;
                            cout << "q or ESC PRESSED, EXITING " << endl; 
                            return 0; 
                   }
            }
            else if (windowEvent.type == SDL_KEYUP){
                   switch (windowEvent.key.keysym.sym){
                       case SDLK_q:
                           cout << "q RELEASED, EXITING " << endl;  
                           return 0; 
                   }
            }
            else {
                for(int instructionCounter = 0; instructionCounter < instructionsPerSecond || cpu.getCurrentInstruction()[0] == 'd'; instructionCounter++){
                        cpu.fetchInstructions(memory); 
                        cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu); 
                }
            }
            this_thread::sleep_for(chrono::seconds(1)); 
        }
    }    

    screen.destroyCreatedWindow(); 

return 0; 

}
