#include <iostream> 
#include "Cpu.h" 
#include "Cpu.cpp" 
#include "Screen.h"
#include "Screen.cpp" 

#include <SDL2/SDL.h> 

using namespace std; 

int main (int argv, char** args){

    Screen screen;  

    screen.initializeScreen(); 

    bool windowIsRunning = true; 
    SDL_Event windowEvent; 

    string romFileLocation = "ROMS/IBM Logo.ch8"; 

    while(windowIsRunning){
        if (SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break; 
            }


            loadDataIntoMemory(fontData, memory); 
            
            cout << "PROGRAM COUNTER after loading font data in Memory " << getProgramCounter() << endl; 

            loadRomIntoMemory(memory, romFileLocation, &programCounter); 

            cout << "PROGRAM COUNTER after loading roms in Memory " << getProgramCounter() << endl; 

            // debug_printMemory(memory);     

            fetchInstructions(memory); 

            cout << "PROGRAM COUNTER after fetching cpu instruction " << getProgramCounter() << endl;  

            decodeAndExecuteInstructions(currentInstruction, screen, memory); 

            fetchInstructions(memory); 

            decodeAndExecuteInstructions(currentInstruction, screen, memory);     

            fetchInstructions(memory); 

            decodeAndExecuteInstructions(currentInstruction, screen, memory);     

            fetchInstructions(memory); 

            decodeAndExecuteInstructions(currentInstruction, screen, memory);     

            fetchInstructions(memory); 

            decodeAndExecuteInstructions(currentInstruction, screen, memory);     

            fetchInstructions(memory); 

            decodeAndExecuteInstructions(currentInstruction, screen, memory);   

            fetchInstructions(memory); 

            decodeAndExecuteInstructions(currentInstruction, screen, memory);   

            fetchInstructions(memory); 

            decodeAndExecuteInstructions(currentInstruction, screen, memory);   

            cout << "PROGRAM COUNTER " << getProgramCounter() << endl;   

            // TODO: delete these later 
            unsigned char testVar = memory[regist_I]; 
            cout << "Sprite to be drawn " << int(testVar) << endl; 
            SDL_Delay(10000); 
        }
    }   

    screen.destroyCreatedWindow(); 

return 0; 

}