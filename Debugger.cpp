#include <iostream> 
#include <chrono> 
#include <thread> 

#include "Debugger.h"
#include "src/include/SDL2/SDL.h" 

using namespace std; 

bool debugWindowIsRunning = true; 

void Debugger::runDebugger(SDL_Event windowEvent, Cpu cpu, Memory memory, Screen screen){
    cout << "[DEBUGGER] Press right arrow key to step 1 instruction, up arrow key to step by 5 instructions" << endl; 
    while(debugWindowIsRunning){
            if (SDL_PollEvent(&windowEvent)){
                if(windowEvent.type == SDL_QUIT){
                            debugWindowIsRunning = false; 
                            cout << "[DEBUGGER] Clicked closed, exiting debugging mode" << endl; 
                            return; 
                }
                else if (windowEvent.type == SDL_KEYDOWN || windowEvent.type == SDL_KEYUP){
                    switch (windowEvent.key.keysym.sym){
                            case SDLK_q: case SDLK_ESCAPE: 
                                debugWindowIsRunning = false;
                                cout << "[DEBUGGER] q or ESC PRESSED, exiting debugging mode" << endl; 
                                return; 
                            case SDLK_RIGHT:
                                cpu.fetchInstructions(memory); 
                                cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu); 
                                break; 
                            case  SDLK_UP:
                                for (int i = 0; i < 5; i++){
                                    cpu.fetchInstructions(memory); 
                                    cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu);    
                                }
                                break; 
                    }
                }
                this_thread::sleep_for(chrono::seconds(1)); 
            }
        }    
}