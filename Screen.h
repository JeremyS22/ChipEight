#ifndef SCREEN_H
#define SCREEN_H 

#include "src/include/SDL2/SDL.h" 
#include "Cpu.h"
class Cpu; 

class Screen {
private: 
    int scalingMultipiler; 
    bool pixelStatus[64][32]; 

public: 
    SDL_Window* window; 
    SDL_Renderer* renderer; 

    Screen(){}; 
    Screen(int scalingMultipiler, SDL_Window* window, SDL_Renderer* renderer, Cpu& cpu){
        this->scalingMultipiler = scalingMultipiler; 
        this->window = window; 
        this->renderer = renderer; 

        turnOffAllPixels(cpu); 
    }
    ~Screen(){}; 

    void initializeScreen(); 
    void turnOffAllPixels(Cpu& cpu); 
    void setScalingMultipiler(int ScalingMultipiler); 
    int getScalingMultipiler();  
    void setPixelStatus(int coordinateX, int coordinateY, bool value, Cpu& cpu); 
    void destroyCreatedWindow(); 
}; 

#endif