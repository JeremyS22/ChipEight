#ifndef SCREEN_H
#define SCREEN_H 

#include <SDL2/SDL.h> 

class Screen {
private: 
    int scalingMultipiler; 
    bool pixelStatus[64][32]; 

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
    void setPixelStatus(bool pixelStatus[64][32]); 
    void destroyCreatedWindow(); 
}; 

#endif