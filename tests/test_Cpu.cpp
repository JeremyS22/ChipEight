#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h>
#include <Memory.h> 
#include <Screen.h> 
#include <Keypad.h> 

#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 
 

TEST(testingSuite1, test1){
    Debugger debugger; 
    Cpu cpu(debugger); 
    cpu.setRegist_V(0, 3);
    cpu.setRegist_V(1, 6);
    int secondNibble = 0; 
    int thirdNibble = 1; 

    cpu.setVXToValueOfVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 6); 
}

TEST(testingSuite1, test2){
    EXPECT_EQ(3+3, 6); 
}

