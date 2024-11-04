#ifndef SCREEN_H
#define SCREEN_H 

#include <SDL2/SDL.h> 

class Screen {
private: 
    // Setting this to 16 by default since Chip 8 native's resoution is so small  
    int scalingMultipiler = 16; 

public: 
    SDL_Window* window; 
    SDL_Renderer* renderer; 

    Screen(){}; 
    Screen(int scalingMultipiler, SDL_Window* window, SDL_Renderer* renderer){
        this->scalingMultipiler = scalingMultipiler; 
        this->window = window; 
        this->renderer = renderer; 
    }
    ~Screen(){}; 

    void initializeScreen(); 
    void setScalingMultipiler(int ScalingMultipiler); 
    int getScalingMultipiler(); 
    void destroyCreatedWindow(); 
}; 

#endif