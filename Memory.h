#ifndef MEMORY_H
#define MEMORY_H  

#include "Cpu.h"
#include <cstdint> 
#include <stack>  
#include <string> 
#include <cstring> 

#include "Cpu.h"

class Cpu; 

class Memory {
    
    private:

    uint16_t stackPointer; 

    // Use for debugging ------------------------------------------------------------------------------- 

    void debug_addSpacing(int i, bool spaceEveryFourNibbles); 
    
    public: 
    
    // 0x000 to 0x1FF are reserved for interpreter 
    // font data at 0x050 - 0x09F  
    // 0x200 to 0xFFF Chip 8 program/data space 
    uint8_t systemMemory[4096];
    std::stack<uint16_t> systemStack; 

    Memory();  
    ~Memory(){}; 

    void loadFontDataIntoMemory(Cpu cpu, Memory& memory); 
    void loadRomIntoMemory(Memory& memory, std::string romFileLocation, Cpu& cpu); 
    void setStackPointer(uint16_t addressAtTopOfStack); 
    uint16_t getStackPointer(); 
    int getStackSize(); 
    
    // Use for debugging ------------------------------------------------------------------------------- 

    void debug_printMemory(Memory memory, bool spaceEveryFourNibbles); 

}; 

#endif 

