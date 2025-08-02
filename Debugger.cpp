#include <iostream> 
#include <chrono> 
#include <thread> 

#include "Debugger.h" 

using namespace std; 

Debugger::Debugger(){} 

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
                            cpu.fetchInstructions(memory, debugger); 
                            cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad, debugger); 
                            break; 
                        case  SDLK_UP:
                            for (int i = 0; i < 5; ++i){
                                cpu.fetchInstructions(memory, debugger); 
                                cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad, debugger);    
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

bool Debugger::initializeDebugger(){

    SDL_Init(SDL_INIT_VIDEO); 
    SDL_Init(SDL_INIT_EVENTS); 

    SDL_CreateWindowAndRenderer(512, 512, (SDL_WINDOW_RESIZABLE), &debuggingWindow, &debuggingRenderer); 
    SDL_SetRenderDrawColor(debuggingRenderer, 0, 0, 0, 0);  
    SDL_RenderClear(debuggingRenderer); 
    SDL_SetWindowPosition(debuggingWindow, 0, 30); 
    SDL_SetWindowTitle(debuggingWindow, "Debugger"); 

    TTF_Init(); 

    createBoxAndAddText(fontSemiBold, "Registers", 10, 20, 70, 20); 

    const char* registerNames[] = {"V0:", "V1:", "V2:", "V3:", "V4:", "V5:", "V6:", "V7:", "V8:", "V9:", "VA:", "VB:", "VC:", "VD:", "VE:", "VF:"}; 
    int height = 50;  
    for(int i = 0; i < 15; i+=2){
        createBoxAndAddText(fontRegular, registerNames[i], 10, height, 20, 20); 
        createBoxAndAddText(fontRegular, registerNames[i+1], 70, height, 20, 20); 
        height+=35; 
    }
    
    createBoxAndAddText(fontSemiBold, "Current Instruction:", 10, 330, 140, 20); 
    createBoxAndAddText(fontSemiBold, "Stack:", 160, 20, 50, 20); 

    SDL_RenderPresent(debuggingRenderer); 

    cout << "Status: (Debugger) Debugging Window and Renderer Created " << endl; 

    return false; 
}

bool Debugger::destroyDebuggerWindow(){
    SDL_DestroyWindow(debuggingWindow); 
    SDL_DestroyRenderer(debuggingRenderer); 
    TTF_CloseFont((messageFont)); 
    TTF_Quit(); 
    SDL_Quit();  
    
    return false; 
}

void Debugger::createBoxAndAddText(const char* font, const char* messageText, int boxPositionX, int boxPositionY, int boxSizeWidth, int boxSizeHeight){

    TTF_Font* messageFont = TTF_OpenFont(font, 28); 

    SDL_Color white = {255, 255, 255}; 

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(messageFont, messageText, white); 

    SDL_Texture* message = SDL_CreateTextureFromSurface(debuggingRenderer, surfaceMessage); 
    SDL_FreeSurface(surfaceMessage); 

    SDL_Rect messageTextbox; 
    messageTextbox.x = boxPositionX; 
    messageTextbox.y = boxPositionY; 
    messageTextbox.w = boxSizeWidth; 
    messageTextbox.h = boxSizeHeight; 

    SDL_RenderCopy(debuggingRenderer, message, NULL, &messageTextbox); 
    
    SDL_RenderPresent(debuggingRenderer); 
    
    SDL_DestroyTexture(message);  
}

void Debugger::outputCurrentInstructionToDebugger(string instruction){
    const char* convertedInstruction = instruction.c_str(); 
    createBoxAndAddText(fontRegular, convertedInstruction, 230, 330, 140, 20); 

    SDL_RenderPresent(debuggingRenderer); 
}

