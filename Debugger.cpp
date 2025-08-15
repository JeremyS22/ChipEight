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

    DebuggingTextbox(fontSemiBold, "Registers", 10, 20, 70, 20, true, getDebuggingRenderer(), "white"); 

    const char* registerNames[] = {"V0:", "V1:", "V2:", "V3:", "V4:", "V5:", "V6:", "V7:", "V8:", "V9:", "VA:", "VB:", "VC:", "VD:", "VE:", "VF:"}; 
    int height = 50;  
    const char* printRegisterValAsZero = "0"; 
    for(int i = 0; i < 15; i+=2){
        DebuggingTextbox(fontRegular, registerNames[i], 10, height, 20, 20, true, getDebuggingRenderer(), "white"); 
        DebuggingTextbox(fontRegular, printRegisterValAsZero, 40, height, 18, 20, true, getDebuggingRenderer(), "white"); 
        DebuggingTextbox(fontRegular, registerNames[i+1], 90, height, 20, 20, true, getDebuggingRenderer(), "white"); 
        DebuggingTextbox(fontRegular, printRegisterValAsZero, 120, height, 18, 20, true, getDebuggingRenderer(), "white"); 
        height+=35; 
    }
    
    DebuggingTextbox(fontSemiBold, "PC:", 10, 330, 30, 20, true, getDebuggingRenderer(), "white"); 
    DebuggingTextbox(fontSemiBold, "Instr:", 10, 350, 50, 20, true, getDebuggingRenderer(), "white"); 
    DebuggingTextbox(fontSemiBold, "Stack:", 160, 20, 50, 20, true, getDebuggingRenderer(), "white"); 

    DebuggingTextbox(fontRegular, "Register I:", 10, 372, 90, 22, true, getDebuggingRenderer(), "white"); 
    DebuggingTextbox(fontExtraBold, "0", 105, 372, 20, 22, false, getDebuggingRenderer(), "white"); 

    DebuggingTextbox(fontRegular, "0x0", 190, 50, 36, 22, true, getDebuggingRenderer(), "white"); 
    DebuggingTextbox(fontRegular, "---", 240, 50, 60, 22, true, getDebuggingRenderer(), "white"); 

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
    DebuggingTextbox(fontExtraBold, instruction, 90, 350, 50, 22, false, getDebuggingRenderer(), "white"); 
    outputPastInstructionsToDebugger(instruction); 
}

void Debugger::outputProgramCounterToDebugger(uint16_t programCounter){
    string convertedProgramCounter = convertIntToHexString(programCounter);  
    DebuggingTextbox(fontExtraBold, convertedProgramCounter, 90, 329, 50, 22, false, getDebuggingRenderer(), "white"); 
    outputPastPCToDebugger(convertedProgramCounter); 
}

void Debugger::outputPastInstructionsToDebugger(string currentInstruction){
    pastInstructionVector.emplace(pastInstructionVector.begin(), currentInstruction); 
    
    if(pastInstructionVector.size() > 6){
        pastInstructionVector.pop_back(); 
    }
        
    int xCoordinate = 150; 
    for(int i = 1; i < pastInstructionVector.size(); i++){
        if(i >= 4){
            DebuggingTextbox(fontExtraBold, pastInstructionVector[i], xCoordinate, 350, 50, 22, false, getDebuggingRenderer(), "dark gray");     
        }
        else {
            DebuggingTextbox(fontExtraBold, pastInstructionVector[i], xCoordinate, 350, 50, 22, false, getDebuggingRenderer(), "gray"); 
        }
        xCoordinate+=60; 
    }
}

void Debugger::outputPastPCToDebugger(string programCounter){
    pastProgramCounterVector.emplace(pastProgramCounterVector.begin(), programCounter); 

    if(pastProgramCounterVector.size() > 6){
        pastProgramCounterVector.pop_back(); 
    }
        
    int xCoordinate = 150; 
    for(int i = 1; i < pastProgramCounterVector.size(); i++){
        if(i >= 4){
            DebuggingTextbox(fontExtraBold, pastProgramCounterVector[i], xCoordinate, 329, 50, 22, false, getDebuggingRenderer(), "dark gray");     
        }
        else {
            DebuggingTextbox(fontExtraBold, pastProgramCounterVector[i], xCoordinate, 329, 50, 22, false, getDebuggingRenderer(), "gray"); 
        }
        xCoordinate+=60; 
    }
}

void Debugger::outputRegisterIToDebugger(string address){
    stringstream sstreamObj; 
    sstreamObj << "0x" << address;
    string convertedAddress = sstreamObj.str(); 
    DebuggingTextbox(fontExtraBold, convertedAddress, 105, 372, 60, 22, false, getDebuggingRenderer(), "white"); 
}

void Debugger::outputRegistersToDebugger(uint8_t registerValue, int registerName){
    string convertedRegisterValue = convertIntToString(registerValue, false, false); 
    int targetPositionX = (registerName % 2 == 0)  ? 40 : 120; 

    // calling function for each register  individually to fix/readjust text size rendering issues 
    switch(registerName){
        case 0x0:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 50, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x1:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 50, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x2:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 85, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x3:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 85, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x4:
            DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 120, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x5:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 120, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x6:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 155, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x7:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 155, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x8:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 190, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0x9:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 190, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0xA:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 225, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0xB:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 225, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0xC:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 260, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0xD:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 260, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0xE:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 295, 20, 22, false, getDebuggingRenderer(), "white"); break; 
        case 0xF:
           DebuggingTextbox(fontExtraBold, convertedRegisterValue, targetPositionX, 295, 20, 22, false, getDebuggingRenderer(), "white"); break; 
    }
} 

void Debugger::outputStackToDebugger(Memory memory){
    string topOfStack = convertIntToHexString(memory.getStackPointer()); 
    if(memory.getStackSize() > stackPrintingVector.size()){
        stackPrintingVector.emplace(stackPrintingVector.begin(), topOfStack); 
    }
    else {
        // deletes text left on screen of popped off stack address  
        int printOverTextY = 50 + ((stackPrintingVector.size() - 1) * 35); 
        DebuggingTextbox(fontExtraBold, "", 190, printOverTextY, 110, 22, false, getDebuggingRenderer(), "white"); 
        stackPrintingVector.erase(stackPrintingVector.begin()); 
    }

    int positionY = 50; 
    int secondPositionY = 50; 
    if(stackPrintingVector.size() == 0){ 
        DebuggingTextbox(fontRegular, "0x0", 190, positionY, 36, 22, false, getDebuggingRenderer(), "white"); 
        DebuggingTextbox(fontRegular, "---", 240, positionY, 60, 22, false, getDebuggingRenderer(), "white"); 
    }
    else {
        for(int i = 0; i < stackPrintingVector.size(); ++i){
            if(i > 7){
                DebuggingTextbox(fontRegular, convertIntToString(i, true, true), 330, secondPositionY, 36, 22, false, getDebuggingRenderer(), "white"); 
                DebuggingTextbox(fontExtraBold, stackPrintingVector.at(i), 380, secondPositionY, 60, 22, false, getDebuggingRenderer(), "white"); 
                secondPositionY+=35; 
            }
            else {
                DebuggingTextbox(fontRegular, convertIntToString(i, true, true), 190, positionY, 36, 22, false, getDebuggingRenderer(), "white"); 
                DebuggingTextbox(fontExtraBold, stackPrintingVector.at(i), 240, positionY, 60, 22, false, getDebuggingRenderer(), "white"); 
                positionY+=35; 
            }       
        }
    }
}

string Debugger::convertIntToString(int value, bool add0xPrefix, bool convertToHexDigits){
    stringstream stringStreamObj; 
    if (add0xPrefix == true){
        if(value > 9 && convertToHexDigits == true){
            value+=55; 
            stringStreamObj << "0x" << char(value); 
        }
        else {
            stringStreamObj << "0x" << value; 
        }
    }
    else {
        stringStreamObj << value; 
    }
    
    string convertedValue = stringStreamObj.str();    
    
    return  convertedValue; 
}

string Debugger::convertIntToHexString(int value){
    stringstream hexadecimalVal; 
    hexadecimalVal << "0x" << hex << value; 

    return hexadecimalVal.str(); 
}

