#include <iostream> 
#include "Cpu.h" 
#include "Cpu.cpp" 

using namespace std; 



int main (){

    string romFileLocation = "ROMS/IBM Logo.ch8"; 

    loadDataIntoMemory(fontData, memory); 
    
    cout << "PROGRAM COUNTER " << programCounter << endl; 

    loadRomIntoMemory(memory, romFileLocation, &programCounter); 

    cout << "PROGRAM COUNTER " << programCounter << endl; 

    printMemory(memory); 

    fetchInstructions(memory); 

    cout << "PROGRAM COUNTER " << programCounter << endl;  

    decodeAndExecuteInstructions(currentInstruction); 

    fetchInstructions(memory); 

    decodeAndExecuteInstructions(currentInstruction);     

    fetchInstructions(memory); 

    decodeAndExecuteInstructions(currentInstruction);     




    

    // TODO: take 16 bit address and let's cpu decode it and excute it 

    // TODO: Figure out where to store opcode instruction, if nowhere, create a value that will 
    // temporary store it 

    // TODO: Start decode function 

    // TODO: Create Cpu class and memory to separate functions and value, make program counter private? 


    // DONE: Cpu fetch, combine 2 memory addresses to get 16 bit address, increment program counter in memory 
    // DONE: Update the Program Counter to be used outside of the function using pass by reference and 
    // to prevent gobbly gook  

    // DONE: might put program counter in a function as a getter for printing, not a setter yet 


return 0; 

}