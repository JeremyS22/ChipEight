#include <iostream> 
#include <chrono> 
#include <thread> 

#include "Debugger.h" 

using namespace std; 

Debugger::Debugger(){} 

void Debugger::runDebugger(Cpu cpu, Memory memory, Screen& screen, Keypad keypad){
    cout << "[DEBUGGER] Press right arrow key to step 1 instruction, up arrow key to step by 5 instructions" << endl; 
    while(screen.getWindowIsOpen()){
            if (SDL_PollEvent(&screen.windowEvent)){
                if(screen.windowEvent.type == SDL_QUIT){
                            screen.setWindowIsOpen(false); 
                            cout << "[DEBUGGER] Clicked closed, exiting debugging mode" << endl; 
                            return; 
                }
                else if (screen.windowEvent.type == SDL_KEYDOWN){
                    switch (screen.windowEvent.key.keysym.sym){
                        case SDLK_ESCAPE: 
                            screen.setWindowIsOpen(false); 
                            cout << "[DEBUGGER] ESC PRESSED, exiting debugging mode" << endl; 
                            return; 
                        case SDLK_RIGHT:
                            cpu.fetchInstructions(memory); 
                            cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad); 
                            break; 
                        case  SDLK_UP:
                            for (int i = 0; i < 5; i++){
                                cpu.fetchInstructions(memory); 
                                cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad);    
                            }
                            break; 
                    }
                }
                this_thread::sleep_for(chrono::seconds(1)); 
            }
        }    
}