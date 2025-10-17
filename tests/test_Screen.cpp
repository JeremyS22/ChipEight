#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h> 
#include <Screen.h> 

#include <iostream> 
#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 

class ScreenTest : public testing::Test {
    protected: 
    ScreenTest() : cpu(debugger){}; 

    Screen screen; 
    Debugger debugger; 
    Cpu cpu; 

    void testPrep_TurnAllPixelsOn(){
        for(int i = 0; i < 64; ++i){
            for(int j = 0; j < 32; ++j){
                screen.setPixelStatus(i,j,true,cpu);         
            }
        }        
    }
}; 

TEST_F(ScreenTest, getPixelStatus_ReturnsFalse_WhenPixelIsFalse){
    const bool value = false; 
    screen.setPixelStatus(16, 28, value, cpu); 

    EXPECT_FALSE(screen.getPixelStatus(16,28)); 
}

TEST_F(ScreenTest, getPixelStatus_ReturnsTrue_WhenPixelIsTrue){
    const bool value = true; 
    screen.setPixelStatus(16, 28, value, cpu); 

    EXPECT_TRUE(screen.getPixelStatus(16,28)); 
}

TEST_F(ScreenTest, setPixelStatus_PixelWillBeSetToTrue_WhenValueTrue_AndExistingPixelFalse){
    const bool value = true; 
    screen.setPixelStatus(20, 26, value, cpu); 

    EXPECT_TRUE(screen.getPixelStatus(20,26)); 
}

TEST_F(ScreenTest, setPixelStatus_PixelWillBeSetToFalse_WhenBothValueAndExistingPixelAreTrue){
    const bool value = true; 
    screen.setPixelStatus(20, 26, value, cpu); 

    screen.setPixelStatus(20, 26, value, cpu); 

    EXPECT_FALSE(screen.getPixelStatus(20,26));
}

TEST_F(ScreenTest, setPixelStatus_VFSetToOne_WhenBothValueAndExistingPixelAreTrue){
    const bool value = true; 
    screen.setPixelStatus(20, 26, value, cpu); 

    screen.setPixelStatus(20, 26, value, cpu); 

    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Incorrect value in register VF, check flag register implementation"; 
}

TEST_F(ScreenTest, turnOffAllPixels_EveryPixelSetToFalse_WhenAllPixelsInitiallyTrue){
    testPrep_TurnAllPixelsOn(); 

    screen.turnOffAllPixels(cpu); 

    for(int i = 0; i < 64; ++i){
        for(int j = 0; j < 32; ++j){
            EXPECT_FALSE(screen.getPixelStatus(i,j)) << "Incorrect Pixel set to true.  Where x is " << i << " and y is " << j; 
        }
    }
}

TEST_F(ScreenTest, initializeScreen_EachSDLSubsystemsIsInitialized){

    screen.initializeScreen(); 
    
    // SDL_WasInit() returns 0 if subsystem wasn't initialized 
    EXPECT_NE(SDL_WasInit(SDL_INIT_TIMER), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_AUDIO), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_VIDEO), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_JOYSTICK), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_GAMECONTROLLER), 0); 
    EXPECT_NE(SDL_WasInit(SDL_INIT_EVENTS), 0); 
}

