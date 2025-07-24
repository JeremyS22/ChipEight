#ifndef KEYPAD_H
#define KEYPAD_H 

#include "src/include/SDL2/SDL.h" 
#include "Screen.h"
class Screen; 

class Keypad {
    public: 

    void getKeypadInput(Screen& screen); 

    Keypad(); 
    ~Keypad(){} 
}; 

#endif 

