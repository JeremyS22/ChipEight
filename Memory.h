#ifndef MEMORY_H
#define MEMORY_H  

#include "Cpu.h"
#include <cstdint> 
#include <stack>  
#include <string> 
class Cpu; 

class Memory {
    
    private:

    // Use for debugging ------------------------------------------------------------------------------- 

    void debug_addSpacing(int i, bool spaceEveryFourNibbles); 
    
    public: 
    
    uint8_t systemMemory[4096];
    // have stack big enough for 16 2 byte address, may make bigger later 
    std::stack<uint16_t> systemStack; 

    Memory(){
        systemMemory[4096]; 
    };  
    ~Memory(){}; 

    void loadFontDataIntoMemory(Cpu cpu, Memory& memory); 

    void loadRomIntoMemory(Memory& memory, std::string romFileLocation, Cpu& cpu); 

    // Use for debugging ------------------------------------------------------------------------------- 

    void debug_printMemory(Memory memory, bool spaceEveryFourNibbles); 

}; 

#endif 

