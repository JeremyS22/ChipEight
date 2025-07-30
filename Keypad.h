#ifndef KEYPAD_H
#define KEYPAD_H 
#include <map> 

#include "src/include/SDL2/SDL.h" 
#include "Screen.h"
class Screen; 
class Debugger; 
class Cpu; 
class Memory; 


class Keypad {
    private: 
    std::map<int, SDL_Scancode> keypadMap; 
    
    public: 

    bool getKeypadInput(Screen& screen, Debugger& debugger, Cpu& cpu, Memory& memory, Keypad& keypad); 
    bool checkIfKeyIsPressed(int keyFromVX); 
    bool checkIfKeyIsNotPressed(int value); 
    uint8_t getPressedKey(Screen& screen); 
    
    Keypad(); 
    ~Keypad(){} 
}; 

#endif 

