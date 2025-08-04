#include <iostream> 
#include <sstream> 
#include <cmath>

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

    createBoxAndAddText(fontSemiBold, "Registers", 10, 20, 70, 20, true); 

    const char* registerNames[] = {"V0:", "V1:", "V2:", "V3:", "V4:", "V5:", "V6:", "V7:", "V8:", "V9:", "VA:", "VB:", "VC:", "VD:", "VE:", "VF:"}; 
    int height = 50;  
    const char* printRegisterValAsZero = "0"; 
    for(int i = 0; i < 15; i+=2){
        createBoxAndAddText(fontRegular, registerNames[i], 10, height, 20, 20, true); 
        createBoxAndAddText(fontRegular, printRegisterValAsZero, 40, height, 18, 20, true); 
        createBoxAndAddText(fontRegular, registerNames[i+1], 90, height, 20, 20, true); 
        createBoxAndAddText(fontRegular, printRegisterValAsZero, 120, height, 18, 20, true); 
        height+=35; 
    }
    
    createBoxAndAddText(fontSemiBold, "Current Instruction:", 10, 330, 140, 20, true); 
    createBoxAndAddText(fontSemiBold, "Stack:", 160, 20, 50, 20, true); 

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

void Debugger::createBoxAndAddText(const char* font, const char* messageText, int boxPositionX, int boxPositionY, int boxSizeWidth, int boxSizeHeight, 
    bool textIsStatic){

    TTF_Font* messageFont = TTF_OpenFont(font, 28); 

    if(!messageFont){
        cout << "Error no TTF font not loaded, check your font file path " << endl;  
        return;        
    }

    SDL_Color white = {255, 255, 255}; 

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(messageFont, messageText, white); 

    SDL_Texture* message = SDL_CreateTextureFromSurface(debuggingRenderer, surfaceMessage); 
    SDL_FreeSurface(surfaceMessage); 

    SDL_Rect messageTextbox; 
    messageTextbox.x = boxPositionX; 
    messageTextbox.y = boxPositionY; 
    messageTextbox.w = boxSizeWidth; 
    messageTextbox.h = boxSizeHeight; 

    if(textIsStatic == false){
        // drawing with renderer's color (black) over previous texture to "clear" it 
        SDL_RenderFillRect(debuggingRenderer, &messageTextbox); 
    }

    SDL_RenderCopy(debuggingRenderer, message, NULL, &messageTextbox); 
    
    SDL_RenderPresent(debuggingRenderer); 
    
    SDL_DestroyTexture(message);  
}

void Debugger::outputCurrentInstructionToDebugger(string instruction){
    const char* convertedInstruction = instruction.c_str(); 
    createBoxAndAddText(fontExtraBold, convertedInstruction, 160, 329, 50, 22, false); 
}

void Debugger::outputRegistersToDebugger(uint8_t registerValue, int registerName){
    const char* convertedRegisterValue = convertIntToCharPointer(registerValue); 
    int targetPositionX = (registerName % 2 == 0)  ? 40 : 120; 
    int targetPositionY; 

    // calling function for each to individually fix/readjust text size rendering issues 
    switch(registerName){
        case 0x0:
            targetPositionY = 50; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x1:
            targetPositionY = 50; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x2:
            targetPositionY = 85; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x3:
            targetPositionY = 85; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x4:
            targetPositionY = 120; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x5:
            targetPositionY = 120; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x6:
            targetPositionY = 155; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x7:
            targetPositionY = 155; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x8:
            targetPositionY = 190; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0x9:
            targetPositionY = 190; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0xA:
            targetPositionY = 225; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0xB:
            targetPositionY = 225; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0xC:
            targetPositionY = 260; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0xD:
            targetPositionY = 260; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0xE:
            targetPositionY = 295; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        case 0xF:
            targetPositionY = 295; 
            createBoxAndAddText(fontExtraBold, convertedRegisterValue, targetPositionX, targetPositionY, 50, 22, false); 
            break; 
        }
} 

const char* Debugger::convertIntToCharPointer(int value){
    stringstream stringStreamObj; 
    stringStreamObj << value; 
    const char* convertedValue = stringStreamObj.str().c_str();    
    
    return  convertedValue; 
}