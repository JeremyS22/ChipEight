#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h>
#include <Memory.h> 
#include <Screen.h> 
#include <Keypad.h> 
#include <mutex>

#include <vector>
#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 

class DebuggerTest : public testing::Test {
    protected: 
    DebuggerTest() : cpu(debugger){
        memory.systemStack.push(200); 
        memory.systemStack.push(256); 
        memory.systemStack.push(300); 
        memory.systemStack.push(4095); 
    }; 

    Screen screen; Keypad keypad; 
    Debugger debugger; 
    Cpu cpu;
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

TEST_F(DebuggerTest, initializeDebugger_EachSDLSubsystemsIsInitialized){

    debugger.initializeDebugger(); 

    // hexadecimal digits represents specific SDL subsystems 
    // https://wiki.libsdl.org/SDL3/SDL_InitFlags#syntax
    EXPECT_EQ(SDL_WasInit(SDL_INIT_VIDEO), 0x00000020u); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_EVENTS), 0x00004000u); 
    EXPECT_NE(SDL_Init(SDL_INIT_VIDEO), -1); 
}

TEST_F(DebuggerTest, initializeDebugger_WindowNamedCorrectTitle){
    debugger.initializeDebugger(); 

    EXPECT_STREQ(SDL_GetWindowTitle(debugger.getDebuggingWindow()), "Debugger"); 
}

