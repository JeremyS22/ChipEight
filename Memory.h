#ifndef MEMORY_H
#define MEMORY_H  

#include <cstdint> 

class Memory {
    
    private:

    // Use for debugging ------------------------------------------------------------------------------- 

    void debug_addSpacing(int i, bool spaceEveryFourNibbles); 

    public: 

    uint8_t systemMemory[4096];

    Memory(){
        systemMemory[4096]; 
    };  
    ~Memory(){}; 

    void loadFontDataIntoMemory(uint8_t fontDataArray[], Memory& memory); 

    void loadRomIntoMemory(Memory& memory, std::string romFileLocation, uint16_t* programCounter); 

    // Use for debugging ------------------------------------------------------------------------------- 

    void debug_printMemory(Memory memory, bool spaceEveryFourNibbles); 

}; 

#endif 

