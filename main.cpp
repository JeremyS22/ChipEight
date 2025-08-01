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
    
    int instructionsPerSecond = 540; 
    
    debugger.setDebuggerIsOn(false); 

    string romFileLocation = "ROMS/Pong 2 (Pong hack) [David Winter, 1997].ch8"; 
        
    screen.initializeScreen(); 

    debugger.initializeDebugger(); 

    memory.loadFontDataIntoMemory(cpu, memory); 

    memory.loadRomIntoMemory(memory, romFileLocation, cpu); 

    if (debugger.getDebuggerIsOn() == true){
        debugger.runDebugger(cpu, memory, screen, keypad);  
    }
    else {
        while(screen.getWindowIsOpen()){
            auto startOfClock = chrono::steady_clock::now(); 
            for(int instructionCounter = 0; instructionCounter < instructionsPerSecond || cpu.getCurrentInstruction()[0] == 'd'; instructionCounter++){ 
                bool inputToCloseEmulator = keypad.getKeypadInput(screen, debugger, cpu, memory, keypad); 
                if(inputToCloseEmulator == true)
                    return 0; 
                cpu.fetchInstructions(memory); 
                cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad); 
            }
            auto endOfClock = chrono::steady_clock::now(); 
            chrono::duration<double> timeElasped = startOfClock - endOfClock; 

            double secondsToWait = 1.0 - timeElasped.count();  
            if (secondsToWait > 0.0 && debugger.getDebuggerIsOn() == false){
                cout << "SLEEPING FOR TIME " << endl; 
                this_thread::sleep_for(chrono::duration<double>(secondsToWait));     
            }
            else {
                debugger.setDebuggerIsOn(false); 
            }
        }
    }
    screen.destroyCreatedWindow(); 
    screen.setWindowIsOpen(false); 

return 0; 

}
