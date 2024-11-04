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

    while(windowIsRunning){
        if (SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break; 
            }
        }
    }   

    screen.destroyCreatedWindow(); 

    

    




    string romFileLocation = "ROMS/IBM Logo.ch8"; 

    loadDataIntoMemory(fontData, memory); 
    
    cout << "PROGRAM COUNTER " << programCounter << endl; 

    loadRomIntoMemory(memory, romFileLocation, &programCounter); 

    cout << "PROGRAM COUNTER " << programCounter << endl; 

    // printMemory(memory); 

    fetchInstructions(memory); 

    cout << "PROGRAM COUNTER " << programCounter << endl;  

    decodeAndExecuteInstructions(currentInstruction); 

    fetchInstructions(memory); 

    decodeAndExecuteInstructions(currentInstruction);     

    fetchInstructions(memory); 

    decodeAndExecuteInstructions(currentInstruction);     

    fetchInstructions(memory); 

    decodeAndExecuteInstructions(currentInstruction);     

    fetchInstructions(memory); 

    decodeAndExecuteInstructions(currentInstruction);     

    cout << "PROGRAM COUNTER " << programCounter << endl;   







    

    // TODO: Implement the clear screen instruction 

    // TODO: take 16 bit address and let's cpu decode it and excute it 

    // TODO: Create Cpu class and memory to separate functions and value, make program counter private? 

    // TODO: Test out the 1nnn jump to address function! 

    // DONE: Connect VSCode's dedebugger to the makefile 
    // DONE: Make a class for the Screen, maybe?  
    // DONE: Test out SDL2 in this project, then put initializing it in the "Initalize function" 
    // DONE: Start decode function 
    // DONE: Figure out where to store opcode instruction, if nowhere, create a value that will 
    // temporary store it 
    // DONE: Cpu fetch, combine 2 memory addresses to get 16 bit address, increment program counter in memory 
    // DONE: Update the Program Counter to be used outside of the function using pass by reference and 
    // to prevent gobbly gook  

    // DONE: might put program counter in a function as a getter for printing, not a setter yet 


return 0; 

}