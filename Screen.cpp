#include "Screen.h" 
#include <iostream> 

using namespace std; 

void Screen::initializeScreen(){
        SDL_Init(SDL_INIT_EVERYTHING); 

        // Initializing this to 16 by default since Chip 8 native's resoution is so small  
        setScalingMultipiler(16); 
        SDL_CreateWindowAndRenderer(64 * getScalingMultipiler(), 32 * getScalingMultipiler(), 0, &window, &renderer); 

        // TODO: Add customizable color background 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
        SDL_RenderClear(renderer); 

        cout << "Status: (Screen) Created Window and Redenerer " << endl; 
} 

void Screen::setScalingMultipiler(int ScalingMultipiler){
        scalingMultipiler = ScalingMultipiler; 
}

int Screen::getScalingMultipiler(){
        return scalingMultipiler; 
}

void Screen::destroyCreatedWindow(){
        // SDL_DestroyWindow(window); 
        //SDL_DestroyRenderer(renderer); 
        SDL_Quit();  
} 