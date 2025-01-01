#ifndef MEMORY_H
#define MEMORY_H  

#include <cstdint> 

class Memory {
    
    public: 

    uint8_t memory[4096];

    Memory(){
        memory[4096]; 
    };  
    ~Memory(){}; 

    void loadDataIntoMemory(uint8_t fontDataArray[], Memory& memory); 

}; 

#endif 

