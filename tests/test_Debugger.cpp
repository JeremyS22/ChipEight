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

        subsystemMap = {
            {"SDL_INIT_TIMER", 0x00000001u},
            {"SDL_INIT_AUDIO", 0x00000010u},
            {"SDL_INIT_VIDEO", 0x00000020u},
            {"SDL_INIT_JOYSTICK", 0x00000200u}, 
            {"SDL_INIT_GAMECONTROLLER", 0x00002000u},
            {"SDL_INIT_EVENTS", 0x00004000u}   
        }; 
    }; 

    Screen screen; Keypad keypad; 
    Debugger debugger; 
    Cpu cpu;
    Memory memory;

    std::unordered_map<std::string, int> subsystemMap; 
    void testPrep_EmptySystemStack(){
        while(!memory.systemStack.empty()){
            memory.systemStack.pop();
        }
    }

    void testPrep_PopFakeDataOnStack(){
        memory.systemStack.push(0x200); 
        memory.systemStack.push(0x226);  
        memory.systemStack.push(0x228); 
        memory.systemStack.push(0x22a);  
        memory.systemStack.push(0x22c); 
        memory.systemStack.push(0x22e);  
        memory.systemStack.push(0x230); 
        memory.systemStack.push(0x232);  
        memory.systemStack.push(0x234); 
        memory.systemStack.push(0x236);  
        memory.systemStack.push(0x238); 
        memory.systemStack.push(0x23a);  
        memory.systemStack.push(0x23c); 
        memory.systemStack.push(0x23e);  
        memory.systemStack.push(0x240); 
        memory.systemStack.push(0x242);  
    }
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
    
    // cleaning up 
    debugger.destroyDebuggerWindow(); 
}

TEST_F(DebuggerTest, initializeDebugger_DebuggerWindowIsVisible_WhenItIsCreated){
    debugger.initializeDebugger(); 

    Uint32 windowIsVisibleFlag = 0x00000004;  
    Uint32 maskedResult = SDL_GetWindowFlags(debugger.getDebuggingWindow()) & windowIsVisibleFlag; 
    EXPECT_EQ(maskedResult, windowIsVisibleFlag); 
    debugger.destroyDebuggerWindow(); 
}

TEST_F(DebuggerTest, initializeDebugger_WindowNamedCorrectTitle){
    debugger.initializeDebugger(); 

    EXPECT_STREQ(SDL_GetWindowTitle(debugger.getDebuggingWindow()), "Debugger"); 
    debugger.destroyDebuggerWindow(); 
}

TEST_F(DebuggerTest, initializeDebugger_TTFWasInitialized){

    debugger.initializeDebugger(); 

    // method returns total of times TTF_Init() was called.  Since method above calls this, expected total should be 1  
    EXPECT_EQ(TTF_WasInit(), 1); 
    debugger.destroyDebuggerWindow(); 
}

TEST_F(DebuggerTest, destroyDebuggerWindow_EachSDLSubsystemsIsClosed){
    SDL_Init(SDL_INIT_VIDEO); 
    SDL_Init(SDL_INIT_EVENTS); 

    debugger.destroyDebuggerWindow(); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_VIDEO), 0); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_EVENTS), 0); 
}

TEST_F(DebuggerTest, destroyDebuggerWindow_TTFWasClosed){
    TTF_Init(); 

    debugger.destroyDebuggerWindow(); 
    EXPECT_EQ(TTF_WasInit(), 0); 
}

TEST_F(DebuggerTest, destroyDebuggerWindow_DebuggingWindowIsDestroyed){
    TTF_Init(); 

    debugger.destroyDebuggerWindow(); 
    EXPECT_EQ(TTF_WasInit(), 0); 
}

TEST_F(DebuggerTest, copyStackToDebuggingVector_SizeOfVectorAndStackMatch){
    testPrep_EmptySystemStack(); 

    memory.systemStack.push(0x200); 
    memory.systemStack.push(0x226); 
    
    debugger.copyStackToDebuggingVector(memory); 
    EXPECT_EQ(debugger.getStackPrintingVectorSize(), 2); 
}

TEST_F(DebuggerTest, copyStackToDebuggingVector_EachStackElementsCopiedToVector){
    testPrep_EmptySystemStack(); 

    testPrep_PopFakeDataOnStack();     
    
    debugger.copyStackToDebuggingVector(memory); 
    for(int i = debugger.getStackPrintingVectorSize() - 1; i >= 0; --i){
        EXPECT_EQ(stoi(debugger.getStackPrintingVectorElement(i)), memory.getStackPointer()) << "Error at index " << i << ", Vector element is " << debugger.getStackPrintingVectorElement(i) << "Top of stack " << memory.getStackPointer(); 
        memory.systemStack.pop(); 
    }
}

