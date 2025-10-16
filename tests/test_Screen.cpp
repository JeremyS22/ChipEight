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
}; 

TEST_F(ScreenTest, setPixelStatus_PixelWillBeSetToTrue_WhenValueTrue_AndExistingPixelFalse){
    bool value = true; 
    screen.setPixelStatus(20, 26, value, cpu); 

    EXPECT_EQ(screen.getPixelStatus(20,26), true); 
}

TEST_F(ScreenTest, setPixelStatus_PixelWillBeSetToFalse_WhenBothValueAndExistingPixelAreTrue){
    bool value = true; 
    screen.setPixelStatus(20, 26, value, cpu); 

    screen.setPixelStatus(20, 26, value, cpu); 

    EXPECT_EQ(screen.getPixelStatus(20,26), false);
}

TEST_F(ScreenTest, setPixelStatus_VFSetToOne_WhenBothValueAndExistingPixelAreTrue){
    bool value = true; 
    screen.setPixelStatus(20, 26, value, cpu); 

    screen.setPixelStatus(20, 26, value, cpu); 

    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Incorrect value in register VF, check flag register implementation"; 
}

