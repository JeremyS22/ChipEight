#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h>
#include <Screen.h> 
#include <ChipEight.h> 

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
    chipEight.initializeSDLSubsystems(); 
    
    // SDL_WasInit() returns 0 if subsystem wasn't initialized 
    EXPECT_NE(SDL_WasInit(SDL_INIT_TIMER), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_AUDIO), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_VIDEO), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_JOYSTICK), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_GAMECONTROLLER), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_EVENTS), 0); 
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

