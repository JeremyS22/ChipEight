#include <iostream> 
#include "DebuggingTextbox.h" 

using namespace std; 

DebuggingTextbox::DebuggingTextbox() 
    : font(nullptr), messageText(nullptr), x(0), y(0), width(0), height(0), textIsStatic(false), textColor(""){}; 

DebuggingTextbox::DebuggingTextbox(const char* font, string text, int x, int y, int width, int height, bool textIsStatic, SDL_Renderer* DebuggingRenderer, 
    string textColor) 
    : font(font), x(x), y(y), width(width), height(height), textIsStatic(textIsStatic), textColor(textColor){
        // converting to const char* here to avoid a pointer pointing to deleted data in memory from leaving Debugger class' scope 
        messageText = text.c_str(); 
        createBoxAndAddText(DebuggingRenderer); 
}; 

void DebuggingTextbox::destroyMessageFont(){
    TTF_CloseFont((messageFont)); 
}

void DebuggingTextbox::createBoxAndAddText(SDL_Renderer* passedInDebuggingRenderer){
    SDL_Color pickedColor; 

    TTF_Font* messageFont = TTF_OpenFont(font, 28); 

    if(!messageFont){
        cout << "Error no TTF font not loaded, check your font file path " << endl;  
        return;        
    }

    if(textColor == "gray"){
        pickedColor = {128, 128, 128}; 
    }
    else if (textColor == "dark gray"){
        pickedColor = {64, 64, 64}; 
    }
    else {
        pickedColor = {255, 255, 255}; 
    }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(messageFont, messageText, pickedColor); 

    SDL_Texture* message = SDL_CreateTextureFromSurface(passedInDebuggingRenderer, surfaceMessage); 
    SDL_FreeSurface(surfaceMessage); 

    SDL_Rect messageTextbox; 
    messageTextbox.x = x; 
    messageTextbox.y = y; 
    messageTextbox.w = width; 
    messageTextbox.h = height; 

    if(textIsStatic == false){
        // drawing with renderer's color (black) over previous texture to "clear" it 
        SDL_RenderFillRect(passedInDebuggingRenderer, &messageTextbox); 
    }

    SDL_RenderCopy(passedInDebuggingRenderer, message, NULL, &messageTextbox); 
    
    SDL_RenderPresent(passedInDebuggingRenderer); 
    
    SDL_DestroyTexture(message);  
}

