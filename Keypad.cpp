#include <iostream> 

#include "Keypad.h" 
#include "Screen.h" 

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


char Keypad::getKeypadInput(Screen& screen){
    if (SDL_PollEvent(&screen.windowEvent)){
        if (screen.windowEvent.type == SDL_KEYDOWN || screen.windowEvent.type == SDL_KEYUP){
            switch (screen.windowEvent.key.keysym.sym){
                case SDLK_1: 
                    cout << "1 read " << endl; 
                    return'1'; 
                case SDLK_2: 
                    cout << "2 read " << endl; 
                    return'2'; 
                case SDLK_3: 
                    cout << "3 read " << endl; 
                    return'3'; 
                default:
                    return '\0'; 
            }
        } 
    } 
    return '\0'; 
}

bool Keypad::checkIfKeyIsPressed(int keyFromVX){
    const Uint8* state = SDL_GetKeyboardState(NULL); 

    if(state[keypadMap[keyFromVX]] == true){
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
        || COSMAC_VIP_FLAG_IS_ON == true && SDL_PollEvent(&screen.windowEvent) && screen.windowEvent.type == SDL_KEYUP){ // for COSMAC VIP interpreter 
        cout << "WINDOW EVENT FOUND " << endl; 
        const Uint8* state = SDL_GetKeyboardState(NULL); 
        int keyPressed; 

        for(auto mapIterator : keypadMap){
            if(mapIterator.second == screen.windowEvent.key.keysym.scancode){
                cout << "FOUND KEY, returning hexadecimal value " << endl; 
                return mapIterator.first; 
            }
        }
    }
    return '\0'; 
}

