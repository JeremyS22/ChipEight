#include <iostream> 

#include "Screen.h" 

using namespace std; 

Screen::Screen() : scalingMultipiler(0), windowStatus(false), window(nullptr), renderer(nullptr){
        memset(pixelStatus, false, sizeof(pixelStatus)); 
}

void Screen::initializeScreen(){
             
        SDL_Init(SDL_INIT_TIMER); 
        SDL_Init(SDL_INIT_AUDIO); 
        SDL_Init(SDL_INIT_VIDEO); 
        SDL_Init(SDL_INIT_JOYSTICK); 
        SDL_Init(SDL_INIT_GAMECONTROLLER); 
        SDL_Init(SDL_INIT_EVENTS); 
        
        // Initializing this to 16.0 by default since Chip 8 native's resoution is so small  
        setScalingMultipiler(16.0); 
        SDL_CreateWindowAndRenderer(64 * (int)getScalingMultipiler(), 32 * (int)getScalingMultipiler(), 0, &window, &renderer); 
        SDL_SetWindowTitle(window, "ChipEight"); 

        setWindowIsOpen(true); 
        
        // TODO: Add customizable color background 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);  
        SDL_RenderClear(renderer); 
        cout << "Status: (Screen) Created Window and Renderer " << endl; 

        SDL_RenderPresent(renderer); 
} 

void Screen::turnOffAllPixels(Cpu& cpu){
        /* for (int i = 0; i < 64; ++i){
                for (int j = 0; j < 32; ++j){
                        // initialized to FALSE by default to represent turned off pixels 
                        setPixelStatus(i, j, 0, cpu); 
                }
        }    */
      memset(pixelStatus, false, sizeof(pixelStatus));  
}

void Screen::setScalingMultipiler(float ScalingMultipiler){
        scalingMultipiler = ScalingMultipiler; 
}

float Screen::getScalingMultipiler(){
        return scalingMultipiler; 
}

void Screen::setPixelStatus(int coordinateX, int coordinateY, bool value, Cpu& cpu){
        if (value == 1 && pixelStatus[coordinateX][coordinateY] == 0){
                pixelStatus[coordinateX][coordinateY] = value; 
        }
        else if (value == 1 && pixelStatus[coordinateX][coordinateY] == 1){
                pixelStatus[coordinateX][coordinateY] = 0; 
                cpu.setRegist_V(0xF, 1); 
        }
}

bool Screen::getPixelStatus(int coordinateX, int coordinateY){
        if (pixelStatus[coordinateX][coordinateY] == 0) 
            return false; 
        else
            return true; 
}

void Screen::setWindowIsOpen(bool passedInWindowStatus){
        windowStatus = passedInWindowStatus; 
}

bool Screen::getWindowIsOpen(){
        return windowStatus; 
}

void Screen::destroyCreatedWindow(){
        setWindowIsOpen(false); 
        SDL_DestroyWindow(window); 
        SDL_DestroyRenderer(renderer); 
        SDL_Quit();  
} 
