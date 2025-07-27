#ifndef KEYPAD_H
#define KEYPAD_H 
#include <map> 

#include "src/include/SDL2/SDL.h" 
#include "Screen.h"
class Screen; 

class Keypad {
    private: 
    std::map<int, SDL_Scancode> keypadMap; 
    
    public: 

    char getKeypadInput(Screen& screen); 
    bool checkIfKeyIsPressed(int keyFromVX); 
    bool checkIfKeyIsNotPressed(int value); 
    uint8_t getPressedKey(Screen& screen); 
    
    Keypad(); 
    ~Keypad(){} 
}; 

#endif 

