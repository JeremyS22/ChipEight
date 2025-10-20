#include "ChipEight.h"
#include <chrono> 
#include <thread> 

using namespace std; 

ChipEight::ChipEight() : inputToCloseEmulator(false){}; 

void ChipEight::initializeEmulator(Memory& memory, string romFileLocation, Cpu& cpu, Screen& screen, Debugger& debugger){
    initializeSDLSubsystems(); 
    screen.initializeScreen(); 
    debugger.initializeDebugger(); 
    memory.loadFontDataIntoMemory(cpu, memory); 
    memory.loadRomIntoMemory(memory, romFileLocation, cpu); 
}

bool ChipEight::mainLoop(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger& debugger, int instructionsPerSecond){
    while(true){
        if (debugger.getDebuggerIsOn() == true && debugger.checkDebuggerRunsWithoutStepping() == false){
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

void ChipEight::initializeSDLSubsystems(){
    map<int, Uint32> subSystemMap = {
        {1, SDL_INIT_TIMER},
        {2, SDL_INIT_AUDIO},
        {3, SDL_INIT_VIDEO},
        {4, SDL_INIT_JOYSTICK},
        {5, SDL_INIT_GAMECONTROLLER},
        {6, SDL_INIT_EVENTS}
    }; 
    
    for(auto subSystem : subSystemMap){
        SDL_Init(subSystem.second); 
        if(SDL_WasInit(subSystem.second) == 0){
            cout << "(ChipEight) Error initializing SDL subsystem number " << subSystem.first << ", flag: " << subSystem.second << endl; 
        }
    }
}

void ChipEight::destroyEmulator(Debugger& debugger, Screen& screen, Cpu& cpu){
    waitForDelayTimerThreadToEnd(cpu); 
    debugger.destroyDebuggerWindow();
    screen.destroyCreatedWindow(); 
}

void ChipEight::waitForDelayTimerThreadToEnd(Cpu& cpu){

    if(cpu.checkDelayTimerThreadIsRunning() == false){
        return; 
    }
    else{
        future<bool>& future = cpu.getFuture(); 
        future_status status; 

        while(true){    
            this_thread::sleep_for(chrono::milliseconds(50)); 
            status = future.wait_for(chrono::milliseconds(1)); 
            cout << "Status: (ChipEight) Main thread waiting . . . " << endl; 

            if(status == future_status::ready){

                cout << "Status: (ChipEight) Future received it's result and confirmed runDelayTimer()'s thread has completed, closing emulator" << endl; 
                break; 
            }
        }
    }
}