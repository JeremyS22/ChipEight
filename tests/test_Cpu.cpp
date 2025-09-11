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

// 5xy0 

TEST_F(CpuTest, 5xy0_ProgramCounterIncrementsByTwo_WhenVXEqualVY){
    cpu.setProgramCounter(cpu.getProgramCounterPointer(), 0x200); 
    
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 10);    

    cpu.skipInstructionIfVXEqualsVY(secondNibble, thirdNibble); 

    EXPECT_EQ(cpu.getProgramCounter(), 0x202) << "Program Counter was expected to increment by 2, check conditional logic in 5xy0 instruction"; 
}

TEST_F(CpuTest, 5xy0_ProgramCounterRemainsSame_WhenVXNotEqualVY){
    cpu.setProgramCounter(cpu.getProgramCounterPointer(), 0x200); 
    
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 20);    

    cpu.skipInstructionIfVXEqualsVY(secondNibble, thirdNibble); 

    EXPECT_EQ(cpu.getProgramCounter(), 0x200) << "Program Counter was expected to not increment by 2, check conditional logic in 5xy0 instruction"; 
}

// 7xnn 

TEST_F(CpuTest, 7xnn_VXHoldsCorrectValue_AfterAddingNN){
    cpu.setRegist_V(0, 10);
    std::string value = "a"; 

    cpu.addValueToRegisterVX (secondNibble, value); 
    EXPECT_EQ(cpu.getRegist_V(0), 20) << "Wrong value after adding, check 7xnn's arithmetic implementation.  Remember NN is hexadecimal";   
}

TEST_F(CpuTest, 7xnn_VXOverflowsAndHoldsCorrectValue_WhenAddingMaxValueOfNN){
    cpu.setRegist_V(0, 10);
    std::string value = "ff"; 

    cpu.addValueToRegisterVX (secondNibble, value); 
    EXPECT_EQ(cpu.getRegist_V(0), 9) << "Wrong value after adding, check 7xnn's arithmetic implementation.  Or in the case VX didn't overflow correctly, check VX's data type (which should be uint8_t).  Remember NN is hexadecimal";   
}

TEST_F(CpuTest, 7xnn_VXHoldsCorrectValue_WhenAddingZero){
    cpu.setRegist_V(0, 255);
    std::string value = "0"; 

    cpu.addValueToRegisterVX (secondNibble, value); 
    EXPECT_EQ(cpu.getRegist_V(0), 255) << "Wrong value after adding zero to VX, check 7xnn's arithmetic implementation.  Remember NN is hexadecimal";   
}

TEST_F(CpuTest, 7xnn_VXOverflowsAndHoldsCorrectValue_WhenAddingToMaxValueOfVX){
    cpu.setRegist_V(0, 255);
    std::string value = "a"; 

    cpu.addValueToRegisterVX (secondNibble, value); 
    EXPECT_EQ(cpu.getRegist_V(0), 9) << "Wrong value after adding max NN value to VX, check 7xnn's arithmetic implementation.  Or in the case VX didn't overflow correctly, check VX's data type (which should be uint8_t).  Remember NN is hexadecimal";   
}

TEST_F(CpuTest, 7xnn_VXHoldsCorrectValue_WhenVXIsZero){
    cpu.setRegist_V(0, 0);
    std::string value = "a"; 

    cpu.addValueToRegisterVX (secondNibble, value); 
    EXPECT_EQ(cpu.getRegist_V(0), 10) << "Wrong value after adding where VX is 0, check 7xnn's arithmetic implementation.  Remember NN is hexadecimal";   
}

TEST_F(CpuTest, 7xnn_VXHoldsCorrectValue_WhenNNHasLeadingZeros){
    cpu.setRegist_V(0, 0);
    std::string value = "0a"; 

    cpu.addValueToRegisterVX (secondNibble, value); 
    EXPECT_EQ(cpu.getRegist_V(0), 10) << "Wrong value after adding VX to a NN value with leading zero (ie 0x0a), check 7xnn's arithmetic implementation.  Remember NN is hexadecimal";   
}

// 8xy0 

TEST_F(CpuTest, 8xy0_VXHoldsValueOfVY_WhenSettingVYToVX){
    cpu.setRegist_V(0, 3);
    cpu.setRegist_V(1, 6);

    cpu.setVXToValueOfVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 6) << "VX and VY don't equal each other, check 8xy0's implementation";  
}

// 8xy1 

TEST_F(CpuTest, 8xy1_VXHoldsCorrectValue_WhenBitwiseORVXAndVY){
    cpu.setRegist_V(0, 90);
    cpu.setRegist_V(1, 60);

    cpu.bitwiseOrVXAndVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 126) << "Wrong value, check basic OR implementation in 8xy1"; 
}

TEST_F(CpuTest, 8xy1_TestVXHoldsCorrectValue_WhenVXAndVYHaveSameValue){
    cpu.setRegist_V(0, 20);

    cpu.bitwiseOrVXAndVY(secondNibble, secondNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 20) << "Wrong value where both VX and VY held same value, check OR implementation in 8xy1."; 
}

TEST_F(CpuTest, 8xy1_VXHoldsCorrectValue_WhenTestingORWithZero){
    cpu.setRegist_V(0, 15);
    cpu.setRegist_V(1, 0);

    cpu.bitwiseOrVXAndVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 15) << "Wrong value where VY was zero, check OR implementation in 8xy1."; 
}

TEST_F(CpuTest, 8xy1_VXHoldsCorrectValue_WhenVXAndVYHaveMaxValues){
    cpu.setRegist_V(0, 255);

    cpu.bitwiseOrVXAndVY(secondNibble, secondNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 255) << "Wrong value where both VX and VY held max values, check OR implementation in 8xy1.";  
}

// 8xy4 

TEST_F(CpuTest, 8xy4_VXHoldsSum_WhenVYPlusVX){
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

TEST_F(CpuTest, 8xy4_VXOverflowsCorrectly_AfterAddingVYToVX){
    cpu.setRegist_V(0, 255);
    cpu.setRegist_V(1, 1);

    cpu.addVXToVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "Check overflowing implementation";  
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register 0xF wasn't set to 1, check 0xF in 8xy4's implementation to ensure overflow triggers carry flag"; 
} 

// 8xy5 

TEST_F(CpuTest, 8xy5_SetVXToExpectedValue_WhenSubtractingVYFromVX){
    cpu.setRegist_V(0, 148);
    cpu.setRegist_V(1, 60);
    
    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 88) << "Result should be 88.  Check the substraction between the two operands or opcode implementation"; 
}

TEST_F(CpuTest, 8xy5_VXUnderflowsCorrectly_WhenSubtractingVYFromVX_AndVXIsLessThanVY){
    cpu.setRegist_V(0, 0);
    cpu.setRegist_V(1, 255);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 1) << "Register V1 should underflow to be 1.  ";  
}

TEST_F(CpuTest, 8xy5_SetVFToZero_WhenSubtractingVYFromVX_AndVXIsLessThanVY){
    cpu.setRegist_V(0, 1);
    cpu.setRegist_V(1, 255);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but shouldn't be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but should be 0. Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

TEST_F(CpuTest, 8xy5_SetVFToOne_WhenSubtractingVYFromVX_AndVXIsGreaterThanVY){
    cpu.setRegist_V(0, 2);
    cpu.setRegist_V(1, 1);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but shouldn't be 0.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but should be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

// 8xy6 

TEST_F(CpuTest, 8xy6_VXHoldsCorrectValue_WhenShiftedRightOnce){
    cpu.setRegist_V(0, 0);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "VX should be 0, but received incorrect value after shifting right. Check 8xy6's bitwise implementation";    

    cpu.setRegist_V(0, 7);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_EQ(cpu.getRegist_V(0), 3) << "VX should be 3, but received incorrect value after shifting right. Check 8xy6's bitwise implementation";    

    cpu.setRegist_V(0, 14); 
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_EQ(cpu.getRegist_V(0), 7) << "VX should be 7, but received incorrect value after shifting right. Check 8xy6's bitwise implementation";    

    cpu.setRegist_V(0, 128); 
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_EQ(cpu.getRegist_V(0), 64) << "VX should be 64, but received incorrect value after shifting right. Check 8xy6's bitwise implementation";    
}

TEST_F(CpuTest, 8xy6_VFHoldsShiftedOutBit_WhenVXShiftedRight){
    cpu.setRegist_V(0, 15);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "VF should be 0. Check 0xF in 8xy6's implementation";    
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "VF should be 1. Check 0xF in 8xy6's implementation";    

    cpu.setRegist_V(0, 14);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "VF should be 1.  Check 0xF in 8xy6's implementation";    
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "VF should be 0.  Check 0xF in 8xy6's implementation";    
}

TEST_F(CpuTest, 8xy6_VXSetToVY_WhenCosmacFlagIsOn){
    cpu.setRegist_V(0, 128);
    cpu.setRegist_V(1, 0);
    
    bool turnOnCosmacFlag = true; 
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_EQ(cpu.getRegist_V(0), cpu.getRegist_V(1)) << "VX != VY, when Cosmac flag is on.  Check Cosmac implementation in 8xy6's method";    
}

TEST_F(CpuTest, 8xy6_VXSetToVYAndShiftsCorrectly_WhenCosmacFlagIsOn){
    cpu.setRegist_V(0, 128);
    cpu.setRegist_V(1, 4);
    
    bool turnOnCosmacFlag = true; 
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_EQ(cpu.getRegist_V(0), 2) << "VX holds incorrect value and should be 2.  Either VX wasn't set to VY or VX shifted incorrectly. Check the implementation of either. ";  
}

TEST_F(CpuTest, 8xy6_VFHoldsShiftedOutBit_WhenCosmacFlagIsOn){
    cpu.setRegist_V(0, 128);
    cpu.setRegist_V(1, 4);
    
    bool turnOnCosmacFlag = true; 
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "VF should be 1. Check 0xF in 8xy6's implementation";    
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "VF should be 0. Check 0xF in 8xy6's implementation";    

    cpu.setRegist_V(0, 128);
    cpu.setRegist_V(1, 15);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "VF should be 1. Check 0xF in 8xy6's implementation";    
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "VF should be 0. Check 0xF in 8xy6's implementation";    
}

// 8xy7 

TEST_F(CpuTest, 8xy7_SetVXToExpectedValue_WhenSubtractingVXFromVY){
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 20);
    
    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 10) << "Result should be 88.  Check the substraction between the two operands or opcode implementation"; 
}

TEST_F(CpuTest, 8xy7_VXUnderflowsCorrectly_WhenSubtractingVXFromVY_AndVYIsLessThanVX){
    cpu.setRegist_V(0, 255);
    cpu.setRegist_V(1, 0);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 1) << "Register V1 should underflow to be 1.  ";  
}

TEST_F(CpuTest, 8xy7_SetVFToZero_WhenSubtractingVXFromVY_AndVYIsLessThanVX){
    cpu.setRegist_V(0, 255);
    cpu.setRegist_V(1, 1);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but shouldn't be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but should be 0. Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

TEST_F(CpuTest, 8xy7_SetVFToOne_WhenSubtractingVXFromVY_AndVYIsGreaterThanVX){
    cpu.setRegist_V(0, 1);
    cpu.setRegist_V(1, 255);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but shouldn't be 0.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.getRegist_V(0xF) << "but should be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

// 9xy0 

TEST_F(CpuTest, 9xy0_ProgramCounterIncrementsByTwo_WhenVXNotEqualVY){
    cpu.setProgramCounter(cpu.getProgramCounterPointer(), 0x200); 
    
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 20);    

    cpu.skipInstructionIfVXNotEqualsVY(secondNibble, thirdNibble); 

    EXPECT_NE(cpu.getProgramCounter(), 0x200) << "Program Counter was expected to increment by 2, check conditional logic in 9xy0 instruction"; 
    EXPECT_EQ(cpu.getProgramCounter(), 0x202) << "Program Counter was expected to increment by 2, check conditional logic in 9xy0 instruction"; 
}

TEST_F(CpuTest, 9xy0_ProgramCounterRemainsSame_WhenVXEqualVY){
    cpu.setProgramCounter(cpu.getProgramCounterPointer(), 0x200); 
    
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 10);    

    cpu.skipInstructionIfVXNotEqualsVY(secondNibble, thirdNibble); 

    EXPECT_EQ(cpu.getProgramCounter(), 0x200) << "Program Counter was expected to not increment by 2, check conditional logic in 9xy0 instruction"; 
}

