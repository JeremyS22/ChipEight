#include <iostream> 
#include "Cpu.h" 
#include "Cpu.cpp" 
#include "Screen.h"
#include "Screen.cpp" 
#include "Memory.h" 
#include "Memory.cpp" 

// #include <SDL2/SDL.h> 

#include "src/include/SDL2/SDL.h" 

using namespace std; 

int main (int argv, char** args){

    Screen screen;  
    Memory memory; 

    screen.initializeScreen(); 

    bool windowIsRunning = true; 
    SDL_Event windowEvent; 

    string romFileLocation = "ROMS/IBM Logo.ch8"; 

    while(windowIsRunning){
        if (SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break; 
            }


            memory.loadFontDataIntoMemory(fontData, memory); 
            
            cout << "PROGRAM COUNTER after loading font data in Memory " << getProgramCounter() << endl; 

            memory.loadRomIntoMemory(memory, romFileLocation, &programCounter); 

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
            unsigned char testVar = memory.memory[regist_I]; 
            cout << "Sprite to be drawn " << int(testVar) << endl; 
            SDL_Delay(10000); 
        }
    }   

    screen.destroyCreatedWindow(); 

return 0; 

}