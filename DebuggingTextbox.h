#ifndef DEBUGGINGTEXTBOX_H
#define DEBUGGINGTEXTBOX_H

#include <SDL_ttf.h> 
#include <string> 
#include "Debugger.h"
class Debugger; 

class DebuggingTextbox {
    private:
        const char* font; 
        const char* messageText; 
        int x; 
        int y; 
        int width;
        int height; 
        bool textIsStatic; 
        std::string textColor;

        TTF_Font* messageFont; 
        
    
    public:
        void createBoxAndAddText(SDL_Renderer* passedInDebuggingRenderer); 

        void destroyMessageFont(); 
        
        DebuggingTextbox();
        DebuggingTextbox(const char* font, const char* text, int x, int y, int width, int height, bool textIsStatic); 
        // DebuggingTextbox(const char* font, const char* text, int x, int y, int width, int height, bool textIsStatic, SDL_Renderer* DebuggingRenderer) 
        DebuggingTextbox(const char* font, std::string text, int x, int y, int width, int height, bool textIsStatic, SDL_Renderer* DebuggingRenderer, 
            std::string textColor); 
        ~DebuggingTextbox(){};
};

#endif