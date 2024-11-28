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

        for (int i = 0; i < 64; i++){
            for (int j = 0; j < 32; j++){
                    // initialized to FALSE by default to represent turned off pixels 
                    pixelStatus[i][j] = 0; 
            }
        }   
    }
    ~Screen(){}; 

    void initializeScreen(); 
    void setScalingMultipiler(int ScalingMultipiler); 
    int getScalingMultipiler();  
    void setPixelStatus(int coordinateX, int coordinateY, bool value); 
    void destroyCreatedWindow(); 
}; 

#endif