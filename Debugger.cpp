#include <iostream> 
#include <chrono> 
#include <thread> 

#include "Debugger.h" 

using namespace std; 

Debugger::Debugger(){} 

bool Debugger::runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad){
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
                            for (int i = 0; i < 5; i++){
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

bool Debugger::initializeDebugger(){

    SDL_Window* debuggingWindow;  
    SDL_Renderer* debuggingRenderer;

    SDL_Init(SDL_INIT_VIDEO); 
    SDL_Init(SDL_INIT_EVENTS); 

    SDL_CreateWindowAndRenderer(1024, 512, (SDL_WINDOW_RESIZABLE), &debuggingWindow, &debuggingRenderer); 
    SDL_SetRenderDrawColor(debuggingRenderer, 0, 0, 0, 0);  
    SDL_RenderClear(debuggingRenderer); 
    SDL_SetWindowTitle(debuggingWindow, "Debugger"); 

    TTF_Init(); 
    
    TTF_Font* font = TTF_OpenFont("./assets/Inter_18pt-Regular.ttf", 18); 
    
    SDL_Color white = {255 ,255, 255}; 

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Testing the ttf ", white); 

    SDL_Texture* message = SDL_CreateTextureFromSurface(debuggingRenderer, surfaceMessage); 

    SDL_Rect messageTextbox; 
    messageTextbox.x = 50; 
    messageTextbox.y = 50; 
    messageTextbox.w = 60; 
    messageTextbox.h = 20; 

    SDL_RenderCopy(debuggingRenderer, message, NULL, &messageTextbox); 

    SDL_RenderPresent(debuggingRenderer); 

    return false; 
}