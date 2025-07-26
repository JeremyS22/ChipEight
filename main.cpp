#include <iostream> 
#include "Cpu.h" 
#include "Screen.h"
#include "Memory.h" 
#include "Cpu.h"
#include "Keypad.h" 
#include "Debugger.h"

#include "src/include/SDL2/SDL.h" 
#include <chrono> 
#include <thread> 

using namespace std; 

int main (int argv, char** args){

    Screen screen;  
    Memory memory; 
    Cpu cpu; 
    Keypad keypad; 
    Debugger debugger; 
    
    // int instructionsPerSecond = 540; 
    int instructionsPerSecond = 20; 
    
    bool debuggerIsOn = false; 

    string romFileLocation = "ROMS/6-keypad.ch8"; 

    screen.initializeScreen(); 

    memory.loadFontDataIntoMemory(cpu, memory); 

    memory.loadRomIntoMemory(memory, romFileLocation, cpu); 

    if (debuggerIsOn == true){
        debugger.runDebugger(cpu, memory, screen, keypad); 
    }
    else {
        while(screen.getWindowIsOpen()){
            if (SDL_PollEvent(&screen.windowEvent)){

                if(screen.windowEvent.type == SDL_QUIT){
                    screen.setWindowIsOpen(false); 
                    cout << "Clicked closed, EXITING " << endl; 
                    return 0; 
                }
                else if (screen.windowEvent.type == SDL_KEYDOWN || screen.windowEvent.type == SDL_KEYUP){
                    switch (screen.windowEvent.key.keysym.sym){
                        case SDLK_ESCAPE: 
                            screen.setWindowIsOpen(false);  
                            cout << "q or ESC pressed or Released, EXITING " << endl; 
                            return 0; 
                    }
                } 
                else {
                    for(int instructionCounter = 0; instructionCounter < instructionsPerSecond || cpu.getCurrentInstruction()[0] == 'd'; instructionCounter++){ 
                        cpu.fetchInstructions(memory); 
                        cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad); 
                    }
                }
                this_thread::sleep_for(chrono::seconds(1)); 
            }
        }    
    }

    screen.destroyCreatedWindow(); 

return 0; 

}
