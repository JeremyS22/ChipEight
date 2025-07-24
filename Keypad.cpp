#include <iostream> 

#include "Keypad.h" 
#include "Screen.h" 

using namespace std; 

Keypad::Keypad(){} 

void Keypad::getKeypadInput(Screen& screen){
    if (SDL_PollEvent(&screen.windowEvent)){

        if(screen.windowEvent.type == SDL_QUIT){
            screen.setWindowIsOpen(false); 
            cout << "Clicked closed, EXITING " << endl; 
            return; 
        }
        else if (screen.windowEvent.type == SDL_KEYDOWN || screen.windowEvent.type == SDL_KEYUP){
            switch (screen.windowEvent.key.keysym.sym){
                    case SDLK_q: case SDLK_ESCAPE: 
                        screen.setWindowIsOpen(false); 
                        cout << "q or ESC pressed or Released, EXITING " << endl; 
                        return; 
            }
        }

    } 
}

