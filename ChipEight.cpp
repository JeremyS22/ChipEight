#include "ChipEight.h"
#include <chrono> 
#include <thread> 

using namespace std; 

ChipEight::ChipEight(){}; 

void ChipEight::initializeEmulator(Memory& memory, string romFileLocation, Cpu& cpu, Screen& screen, Debugger& debugger){
    screen.initializeScreen(); 
    debugger.initializeDebugger(); 
    memory.loadFontDataIntoMemory(cpu, memory); 
    memory.loadRomIntoMemory(memory, romFileLocation, cpu); 
}

bool ChipEight::mainLoop(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger& debugger, bool inputToCloseEmulator, int instructionsPerSecond){
    while(screen.getWindowIsOpen()){
        if (debugger.getDebuggerIsOn() == true){
            inputToCloseEmulator = debugger.runDebugger(cpu, memory, screen, keypad, debugger); 
            if(inputToCloseEmulator == true){
                destroyEmulator(debugger, screen, cpu); 
                return true;  
            }
        }
        auto startOfClock = chrono::steady_clock::now(); 
        for(int instructionCounter = 0; instructionCounter < instructionsPerSecond || cpu.getCurrentInstruction()[0] != 'd'; ++instructionCounter){ 
            inputToCloseEmulator = keypad.getKeypadInput(screen, debugger, cpu, memory, keypad); 
            if(inputToCloseEmulator == true){
                destroyEmulator(debugger, screen, cpu); 
                return true;  
            }
            cpu.fetchInstructions(memory); 
            cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad); 
        }
        auto endOfClock = chrono::steady_clock::now(); 
        chrono::duration<double> timeElasped = startOfClock - endOfClock; 

        double secondsToWait = 1.0 - timeElasped.count();  
        /* if (secondsToWait > 0.0 && debugger.getDebuggerIsOn() == false){
            cout << "SLEEPING FOR TIME " << endl; 
            this_thread::sleep_for(chrono::duration<double>(secondsToWait));     
        } */
    }
    return true; 
}

void ChipEight::destroyEmulator(Debugger& debugger, Screen& screen, Cpu& cpu){
    waitForDelayTimerThreadToEnd(cpu); 
    debugger.destroyDebuggerWindow();
    screen.destroyCreatedWindow(); 
    screen.setWindowIsOpen(false); 
}

void ChipEight::waitForDelayTimerThreadToEnd(Cpu& cpu){
    future<bool>& future = cpu.getFuture(); 
    future_status status; 

    while(true){    
        this_thread::sleep_for(chrono::milliseconds(50)); 
        status = future.wait_for(chrono::milliseconds(1)); 
        cout << "Main thread waiting . . . " << endl; 

        if(status == future_status::ready){
            cout << "Future received it's result and confirmed runDelayTimer()'s thread has completed, closing emulator" << endl; 
            break; 
        }
    }
}