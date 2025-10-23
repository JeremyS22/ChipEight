#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h>
#include <Screen.h> 
#include <ChipEight.h> 
#include <map> 

#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 

class ChipEightTest : public testing::Test {
    protected: 
    ChipEightTest(): cpu(debugger){}

    Screen screen; 
    Debugger debugger; 
    Cpu cpu;

    ChipEight chipEight; 
}; 

TEST_F(ChipEightTest, initializeSDLSubsystems_EachSDLSubsystemsIsInitialized){
    // hexadecimal representations of SDL subsystems (https://ddnet.org/codebrowser/include/SDL2/SDL.h.html)

    chipEight.initializeSDLSubsystems(); 

    EXPECT_EQ(SDL_WasInit(SDL_INIT_TIMER), 0x00000001u); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_AUDIO), 0x00000010u); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_VIDEO), 0x00000020u); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_JOYSTICK), 0x00000200u); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_GAMECONTROLLER), 0x00002000u); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_EVENTS), 0x00004000u);  
}

TEST_F(ChipEightTest, initializeSDLSubsystems_EachSDLSubsystemsIsClosed){
    chipEight.initializeSDLSubsystems(); 

    chipEight.destroyEmulator(debugger, screen, cpu); 
    
    // SDL_WasInit() returns 0 if subsystem wasn't initialized 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_TIMER), 0); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_AUDIO), 0); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_VIDEO), 0); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_JOYSTICK), 0); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_GAMECONTROLLER), 0); 
    EXPECT_EQ(SDL_WasInit(SDL_INIT_EVENTS), 0); 
}

