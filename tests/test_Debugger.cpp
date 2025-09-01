#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h>
#include <Memory.h> 
#include <Screen.h> 
#include <Keypad.h> 

#include <vector>
#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 

class DebuggerTest : public testing::Test {
    protected: 
    DebuggerTest(){
        memory.systemStack.push(200); 
        memory.systemStack.push(256); 
        memory.systemStack.push(300); 
        memory.systemStack.push(4095); 
    }; 

    Debugger debugger; 
    Memory memory;
}; 

TEST_F(DebuggerTest, copyStackToDebuggingVector_ValidatingVectorSize){
    
    debugger.copyStackToDebuggingVector(memory); 

    EXPECT_EQ(debugger.getStackPrintingVectorSize(), 4);   
}   

TEST_F(DebuggerTest, copyStackToDebuggingVector_ValidatingVectorContents){

    debugger.copyStackToDebuggingVector(memory); 

    EXPECT_EQ(debugger.getStackPrintingVectorElement(3),"0xc8");  
    EXPECT_EQ(debugger.getStackPrintingVectorElement(2),"0x100"); 
    EXPECT_EQ(debugger.getStackPrintingVectorElement(0),"0xfff"); 
}
