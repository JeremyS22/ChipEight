#include <gtest/gtest.h> 
#include <Cpu.h> 
#include <Debugger.h>
#include <Memory.h> 
#include <Screen.h> 
#include <Keypad.h> 

#include "src/include/SDL2/SDL.h" 
#include <SDL_ttf.h> 

class CpuTest : public testing::Test {
    protected: 
    CpuTest(): secondNibble('0'), thirdNibble('1'), cpu(debugger){}

    char secondNibble; 
    char thirdNibble; 
    Debugger debugger; 
    Cpu cpu;
}; 

// 8xy0 

TEST_F(CpuTest, RegisterVXEqualsVY){
    cpu.setRegist_V(0, 3);
    cpu.setRegist_V(1, 6);

    cpu.setVXToValueOfVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 6); 
}

// 8xy4 

TEST_F(CpuTest, VXPlusVY){
    cpu.setRegist_V(0, 0);
    cpu.setRegist_V(1, 0);
    cpu.addVXToVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "Result isn't 0, check addition between the two operands or opcode implementation"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF wasn't set to 0, check 0xF in 8xy4's implementation to ensure overflow triggers carry flag"; 

    cpu.setRegist_V(0, 1);
    cpu.setRegist_V(1, 0);
    cpu.addVXToVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 1) << "8xy4 - Result isn't 1, check addition between the two operands or opcode implementation"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF wasn't set to 0, check 0xF in 8xy4's implementation to ensure overflow triggers carry flag"; 
    
    cpu.setRegist_V(0, 0);
    cpu.setRegist_V(1, 1);
    cpu.addVXToVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 1) << "Result isn't 1, check addition between the two operands or opcode implementation"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF wasn't set to 0, check 0xF in 8xy4's implementation to ensure overflow triggers carry flag"; 
    
    cpu.setRegist_V(0, 24);
    cpu.setRegist_V(1, 42);
    cpu.addVXToVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 66) << "Result isn't 66, check addition between the two operands or opcode implementation"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF wasn't set to 0, check 0xF in 8xy4's implementation to ensure overflow triggers carry flag"; 

    cpu.setRegist_V(0, 253);
    cpu.setRegist_V(1, 2);
    cpu.addVXToVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 255) << "Result isn't 255, check addition between the two operands or opcode implementation"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF wasn't set to 0, check 0xF in 8xy4's implementation to ensure overflow triggers carry flag"; 
} 

TEST_F(CpuTest, VXOverflowsCorrectlyAfterAddingVY){
    cpu.setRegist_V(0, 255);
    cpu.setRegist_V(1, 1);

    cpu.addVXToVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "Check overflowing implementation";  
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register 0xF wasn't set to 1, check 0xF in 8xy4's implementation to ensure overflow triggers carry flag"; 
} 

TEST_F(CpuTest, 8xy5_SetVXToExpectedValue_WhenV1MinusV0){
    cpu.setRegist_V(0, 148);
    cpu.setRegist_V(1, 60);
    
    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 88) << "Result should be 88.  Check the substraction between the two operands or opcode implementation"; 
}

TEST_F(CpuTest, 8xy5_VXUnderflowsCorrectly_WhenV0IsLessThanV1){
    cpu.setRegist_V(0, 1);
    cpu.setRegist_V(1, 256);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 1) << "Register V1 should underflow to be 1.  ";  
}

TEST_F(CpuTest, 8xy5_SetVFToZero_WhenV0IsLessThanV1){
    cpu.setRegist_V(0, 1);
    cpu.setRegist_V(1, 255);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but shouldn't be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but should be 0. Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

TEST_F(CpuTest, 8xy5_SetVFToOne_WhenV0IsGreaterThanV1){
    cpu.setRegist_V(0, 2);
    cpu.setRegist_V(1, 1);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but shouldn't be 0.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but should be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

