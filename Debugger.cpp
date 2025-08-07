#include <iostream> 
#include <sstream> 
#include <cmath>

#include "Debugger.h" 

using namespace std; 

Debugger::Debugger() : debuggerIsOn (false), debuggingWindow (nullptr), debuggingRenderer (nullptr), fontRegular("./assets/Inter_28pt-Regular.ttf"), 
    fontExtraBold("./assets/Inter_28pt-ExtraBold.ttf"), fontSemiBold("./assets/Inter_28pt-SemiBold.ttf"){} 

bool Debugger::runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger debugger){
    if (getDebuggerIsOn() == false){
        setDebuggerIsOn(true);
    }
    cout << "[DEBUGGER] Press right arrow key to step 1 instruction, up arrow key to step by 5 instructions" << endl; 
    cout << "[DEBUGGER]  Press Grave/Tilde key (AKA ` or ~) to leave debugger and resume emulation" << endl; 
    cout << "[DEBUGGER]   Press Escape key to close emulator" << endl; 
    while(screen.getWindowIsOpen()){
            if (SDL_PollEvent(&screen.windowEvent)){
                if(screen.windowEvent.type == SDL_QUIT){
                    setDebuggerIsOn(false); 
                    cout << "[DEBUGGER] Clicked closed, exiting debugging mode" << endl; 
                    return true; 
                }
                else if (screen.windowEvent.type == SDL_KEYDOWN){
                    switch (screen.windowEvent.key.keysym.sym){
                        case SDLK_BACKQUOTE: 
                            cout << "[DEBUGGER] Grave/Tilde key PRESSED, exiting debugging mode " << endl; 
                            return false; 
                        case SDLK_RIGHT:
                            cpu.fetchInstructions(memory); 
                            cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad); 
                            break; 
                        case  SDLK_UP:
                            for (int i = 0; i < 5; ++i){
                                cpu.fetchInstructions(memory); 
                                cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad);    
                            }
                            break; 
                        case SDLK_ESCAPE: 
                            setDebuggerIsOn(false); 
                            cout << "[DEBUGGER] ESC PRESSED, exiting emulation" << endl; 
                            return true; 
                        default:
                            break; 
                    }
                }
            }
    }
    return false;   
}

void Debugger::setDebuggerIsOn(bool value){
    debuggerIsOn = value; 
}

bool Debugger::getDebuggerIsOn(){
    return debuggerIsOn; 
}

SDL_Renderer* Debugger::getDebuggingRenderer(){
    return debuggingRenderer; 
}

bool Debugger::initializeDebugger(){

    SDL_Init(SDL_INIT_VIDEO); 
    SDL_Init(SDL_INIT_EVENTS); 

    SDL_CreateWindowAndRenderer(512, 512, (SDL_WINDOW_RESIZABLE), &debuggingWindow, &debuggingRenderer); 
    SDL_SetRenderDrawColor(debuggingRenderer, 0, 0, 0, 0);  
    SDL_RenderClear(debuggingRenderer); 
    SDL_SetWindowPosition(debuggingWindow, 0, 30); 
    SDL_SetWindowTitle(debuggingWindow, "Debugger"); 

    // returns 0 if successful, -1 on any error 
    if(TTF_Init() == -1){
        cout << "Error initializing debugging window, closing SDL and window" << endl; 
        return true; 
    }    

    DebuggingTextbox(fontSemiBold, "Registers", 10, 20, 70, 20, true, getDebuggingRenderer()); 

    const char* registerNames[] = {"V0:", "V1:", "V2:", "V3:", "V4:", "V5:", "V6:", "V7:", "V8:", "V9:", "VA:", "VB:", "VC:", "VD:", "VE:", "VF:"}; 
    int height = 50;  
    const char* printRegisterValAsZero = "0"; 
    for(int i = 0; i < 15; i+=2){
        DebuggingTextbox(fontRegular, registerNames[i], 10, height, 20, 20, true, getDebuggingRenderer()); 
        DebuggingTextbox(fontRegular, printRegisterValAsZero, 40, height, 18, 20, true, getDebuggingRenderer()); 
        DebuggingTextbox(fontRegular, registerNames[i+1], 90, height, 20, 20, true, getDebuggingRenderer()); 
        DebuggingTextbox(fontRegular, printRegisterValAsZero, 120, height, 18, 20, true, getDebuggingRenderer()); 
        height+=35; 
    }
    
    DebuggingTextbox(fontSemiBold, "Current Instruction:", 10, 330, 140, 20, true, getDebuggingRenderer()); 
    DebuggingTextbox(fontSemiBold, "Stack:", 160, 20, 50, 20, true, getDebuggingRenderer()); 

    SDL_RenderPresent(debuggingRenderer); 

    cout << "Status: (Debugger) Debugging Window and Renderer Created " << endl; 

    return false; 
}

bool Debugger::destroyDebuggerWindow(DebuggingTextbox debuggingTextbox){
    SDL_DestroyWindow(debuggingWindow); 
    SDL_DestroyRenderer(debuggingRenderer); 
    debuggingTextbox.destroyMessageFont(); 
    TTF_Quit(); 
    SDL_Quit();  
    
    return false; 
}

void Debugger::outputCurrentInstructionToDebugger(string instruction){
    const char* convertedInstruction = instruction.c_str(); 
    DebuggingTextbox(fontExtraBold, convertedInstruction, 160, 329, 50, 22, false, getDebuggingRenderer()); 
}

void Debugger::outputRegistersToDebugger(uint8_t registerValue, int registerName){
    string convertedRegisterValue = convertIntToCharPointer(registerValue); 
    int targetPositionX = (registerName % 2 == 0)  ? 40 : 120; 

    // calling function for each register  individually to fix/readjust text size rendering issues 
    switch(registerName){
        case 0x0:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 50, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x1:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 50, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x2:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 85, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x3:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 85, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x4:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 120, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x5:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 120, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x6:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 155, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x7:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 155, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x8:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 190, 30, 22, false, getDebuggingRenderer()); break; 
        case 0x9:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 190, 30, 22, false, getDebuggingRenderer()); break; 
        case 0xA:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 225, 30, 22, false, getDebuggingRenderer()); break; 
        case 0xB:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 225, 30, 22, false, getDebuggingRenderer()); break; 
        case 0xC:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 260, 30, 22, false, getDebuggingRenderer()); break; 
        case 0xD:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 260, 30, 22, false, getDebuggingRenderer()); break; 
        case 0xE:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 295, 30, 22, false, getDebuggingRenderer()); break; 
        case 0xF:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 295, 30, 22, false, getDebuggingRenderer()); break; 
    }
} 

string Debugger::convertIntToCharPointer(int value){
    stringstream stringStreamObj; 
    stringStreamObj << value; 
    string convertedValue = stringStreamObj.str();    
    
    return  convertedValue; 
}