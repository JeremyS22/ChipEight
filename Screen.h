#ifndef SCREEN_H
#define SCREEN_H 

#include "src/include/SDL2/SDL.h" 
#include "Cpu.h"

class Cpu; 

class Screen {
private: 
    float scalingMultipiler; 
    bool pixelStatus[64][32]; 
    bool windowStatus; 

public: 
    SDL_Window* window; 
    SDL_Renderer* renderer; 
    SDL_Event windowEvent; 

    Screen(); 
    ~Screen(){}; 

    void initializeScreen(); 
    void turnOffAllPixels(Cpu& cpu); 
    void setScalingMultipiler(float ScalingMultipiler); 
    float getScalingMultipiler();  
    void setPixelStatus(int coordinateX, int coordinateY, bool value, Cpu& cpu); 
    bool getPixelStatus(int coordinateX, int coordinateY); 
    void destroyCreatedWindow(); 
}; 

#endif