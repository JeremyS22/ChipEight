#ifndef KEYPAD_H
#define KEYPAD_H 
#include <map> 

#include "src/include/SDL2/SDL.h" 
#include "Screen.h"
class Screen; 

class Keypad {
    private: 
    std::map<int, SDL_KeyCode> keypadMap; 
    
    public: 

    char getKeypadInput(Screen& screen); 
    bool checkIfKeyIsPressed(Screen& screen, int keyFromVX); 
    bool checkIfKeyIsNotPressed(Screen& screen, int value); 
    
    Keypad(); 
    ~Keypad(){} 
}; 

#endif 

