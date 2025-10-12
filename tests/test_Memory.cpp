#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h>
#include <Memory.h> 

#include <iostream> 
#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 


class MemoryTest : public testing::Test {
    protected: 
    MemoryTest() : cpu(debugger){}; 

    Debugger debugger; 
    Memory memory; 
    Cpu cpu; 
}; 

TEST_F(MemoryTest, loadFontDataIntoMemory_EachAddressHoldsExpectedValue){
    
    memory.loadFontDataIntoMemory(cpu, memory); 

    int j = 0; 
    for(int i = 0x050; i < 160; ++i){
        EXPECT_EQ(memory.getFromMemory(i), cpu.fontData[j]) << "Mismatch error at:\n   Address 0x" << cpu.convertIntToHexString(i) << " in memory and\n   index " << j << " in FontData array";       
        ++j; 
    }
}   

