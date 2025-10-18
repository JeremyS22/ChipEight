#include <iostream> 

#include "Keypad.h" 

using namespace std; 

Keypad::Keypad() : keypadMap({
    {0x0, SDL_SCANCODE_0},
    {0x1, SDL_SCANCODE_1},
    {0x2, SDL_SCANCODE_2},
    {0x3, SDL_SCANCODE_3},
    {0x4, SDL_SCANCODE_4},
    {0x5, SDL_SCANCODE_5},
    {0x6, SDL_SCANCODE_6},
    {0x7, SDL_SCANCODE_7},
    {0x8, SDL_SCANCODE_8},
    {0x9, SDL_SCANCODE_9},
    {0xa, SDL_SCANCODE_A},
    {0xb, SDL_SCANCODE_B},
    {0xc, SDL_SCANCODE_C},
    {0xd, SDL_SCANCODE_D},
    {0xe, SDL_SCANCODE_E},
    {0xf, SDL_SCANCODE_F}
}){}

/**
 *  \returns A boolean determining whether or not the input given was to close emulator, true = close emulator, false = remain open 
*/
bool Keypad::getKeypadInput(Screen& screen, Debugger& debugger, Cpu& cpu, Memory& memory, Keypad& keypad){
    if (SDL_PollEvent(&screen.windowEvent)){
        if(screen.windowEvent.type == SDL_QUIT){
            cout << "Clicked closed, EXITING " << endl; 
            return false; 
        }
        else if (screen.windowEvent.type == SDL_KEYDOWN){
            switch (screen.windowEvent.key.keysym.sym){
                case SDLK_BACKQUOTE: 
                    if(debugger.runDebugger(cpu, memory, screen, keypad, debugger) == true){
                        return true; 
                    }
                    break; 
                case SDLK_ESCAPE: 
                    cout << "Status: (Keypad) ESC pressed or Released, EXITING " << endl; 
                    return true; 
                default:
                    return false; 
            }
        } 
    } 
    return false; 
}

bool Keypad::checkIfKeyIsPressed(int keyFromVX){

    const Uint8* state = SDL_GetKeyboardState(NULL); 
    
    if(state[keypadMap[keyFromVX]] == true){
        cout << "Key " << keyFromVX << " PRESSED " << endl; 
        return true; 
    }
    return false; 
}

bool Keypad::checkIfKeyIsNotPressed(int keyFromVX){
    const Uint8* state = SDL_GetKeyboardState(NULL); 
    
    if(state[keypadMap[keyFromVX]] == false){
        return true; 
    }
    return false; 
}

uint8_t Keypad::getPressedKey(Screen& screen){
    if(SDL_PollEvent(&screen.windowEvent) && COSMAC_VIP_FLAG_IS_ON == false // for modern interpreters 
        || COSMAC_VIP_FLAG_IS_ON == true && screen.windowEvent.type == SDL_KEYUP){ // for COSMAC VIP interpreter 
        cout << "WINDOW EVENT FOUND " << endl; 
        const Uint8* state = SDL_GetKeyboardState(NULL);  

        for(auto mapIterator : keypadMap){
            if(mapIterator.second == screen.windowEvent.key.keysym.scancode){
                cout << "FOUND KEY, returning hexadecimal value " << endl; 
                return mapIterator.first; 
            }
        }
    }
    return '\0'; 
}

