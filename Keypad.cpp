#include <iostream> 

#include "Keypad.h" 
#include "Screen.h" 

using namespace std; 

Keypad::Keypad() : keypadMap({
    {0x0, SDLK_0},
    {0x1, SDLK_1},
    {0x2, SDLK_2},
    {0x3, SDLK_3},
    {0x4, SDLK_4},
    {0x5, SDLK_5},
    {0x6, SDLK_6},
    {0x7, SDLK_7},
    {0x8, SDLK_8},
    {0x9, SDLK_9},
    {0xa, SDLK_a},
    {0xb, SDLK_b},
    {0xc, SDLK_c},
    {0xd, SDLK_d},
    {0xe, SDLK_e},
    {0xf, SDLK_f}
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

bool Keypad::checkIfKeyIsPressed(Screen& screen, int keyFromVX){
    while(SDL_PollEvent(&screen.windowEvent)){
        if(screen.windowEvent.type == SDL_KEYDOWN){
            if(screen.windowEvent.key.keysym.sym == keypadMap[keyFromVX]){
                cout << "KEY FOUND, IS PRESSED " << endl; 
                return true; 
            }
        }
    }
    return false; 
}

bool Keypad::checkIfKeyIsNotPressed(Screen& screen, int keyFromVX){
    while(SDL_PollEvent(&screen.windowEvent)){
        if(screen.windowEvent.type == SDL_KEYUP){
            if(screen.windowEvent.key.keysym.sym == keypadMap[keyFromVX]){
                cout << "KEY FOUND, IS RELEASED" << endl; 
                return true; 
            }
        }
    }
    return false; 
}

