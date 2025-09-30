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

// 8xy2  

TEST_F(CpuTest, 8xy2_VXHoldsCorrectValue_WhenBitwiseANDVXAndVY){
    cpu.setRegist_V(0, 15);
    cpu.setRegist_V(1, 12);

    cpu.bitwiseAndVXAndVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 12) << "Wrong value, check basic AND implementation in 8xy2"; 
}

TEST_F(CpuTest, 8xy2_TestVXHoldsCorrectValue_WhenVXAndVYHaveSameValue){
    cpu.setRegist_V(0, 20);
    
    cpu.bitwiseAndVXAndVY(secondNibble, secondNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 20) << "Wrong value where both VX and VY held same value, check AND implementation in 8xy2."; 
}
    
TEST_F(CpuTest, 8xy2_VXHoldsCorrectValue_WhenTestingANDWithZero){
    cpu.setRegist_V(0, 15);
    cpu.setRegist_V(1, 0);

    cpu.bitwiseAndVXAndVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "Wrong value where VY was zero, check AND implementation in 8xy2."; 
}

TEST_F(CpuTest, 8xy2_VXHoldsCorrectValue_WhenVXAndVYHaveMaxValues){
    cpu.setRegist_V(0, 255);

    cpu.bitwiseAndVXAndVY(secondNibble, secondNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 255) << "Wrong value where both VX and VY held max values, check AND implementation in 8xy2.";  
} 

// 8xy3 

TEST_F(CpuTest, 8xy3_VXHoldsCorrectValue_WhenBitwiseXORVXAndVY){
    cpu.setRegist_V(0, 64);
    cpu.setRegist_V(1, 30);

    cpu.bitwiseExclusiveOrVXAndVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 94) << "Wrong value, check basic XOR implementation in 8xy3"; 
}

TEST_F(CpuTest, 8xy3_TestVXHoldsCorrectValue_WhenVXAndVYHaveSameValue){
    cpu.setRegist_V(0, 128);

    cpu.bitwiseExclusiveOrVXAndVY(secondNibble, secondNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "Wrong value where both VX and VY held same value, check XOR implementation in 8xy3."; 
}

TEST_F(CpuTest, 8xy3_VXHoldsCorrectValue_WhenTestingXORWithZero){
    cpu.setRegist_V(0, 15);
    cpu.setRegist_V(1, 0);

    cpu.bitwiseExclusiveOrVXAndVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 15) << "Wrong value where VY was zero, check XOR implementation in 8xy3."; 
}

TEST_F(CpuTest, 8xy3_VXHoldsCorrectValue_WhenVXAndVYHaveMaxValues){
    cpu.setRegist_V(0, 255);

    cpu.bitwiseExclusiveOrVXAndVY(secondNibble, secondNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "Wrong value where both VX and VY held max values, check XOR implementation in 8xy3.";  
}

TEST_F(CpuTest, 8xy3_VXHoldsCorrectValue_WhenTestingXORWithZero_AndVXHasMaxValue){
    cpu.setRegist_V(0, 255);
    cpu.setRegist_V(1, 0);

    cpu.bitwiseExclusiveOrVXAndVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 255) << "Wrong value where VY was zero and VX was 255, check XOR implementation in 8xy3."; 
}


// 8xy4 

TEST_F(CpuTest, 8xy4_VXHoldsSum_WhenVYPlusVX){
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

TEST_F(CpuTest, 8xy5_VXHoldsExpectedValue){
    cpu.setRegist_V(0, 148);
    cpu.setRegist_V(1, 60);
    
    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 88) << "Result should be 88.  Check the substraction between the two operands or opcode implementation"; 
}

TEST_F(CpuTest, 8xy5_VXUnderflowsCorrectly_WhenVXIsLessThanVY){
    cpu.setRegist_V(0, 0);
    cpu.setRegist_V(1, 255);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 1) << "Register V1 should underflow to be 1.  ";  
}

TEST_F(CpuTest, 8xy5_VXHoldsCorrectValue_WhenVXAndVYAreSameValue){
    cpu.setRegist_V(0, 20);
    cpu.setRegist_V(1, 20);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "The result should be 0 but wasn't.  Check 8xy5's implementation when VX and VY are the same value ";  
}

TEST_F(CpuTest, 8xy5_VFHoldsCorrectValue_WhenVXAndVYAreSameValue){
    cpu.setRegist_V(0, 20);
    cpu.setRegist_V(1, 20);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register VF should be 1 but wasn't.  Check 8xy5's VF logic";  
}

TEST_F(CpuTest, 8xy5_SetVFToZero_WhenVXIsLessThanVY){
    cpu.setRegist_V(0, 1);
    cpu.setRegist_V(1, 255);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.printRegist_V(0xF) << "but shouldn't be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.printRegist_V(0xF) << "but should be 0. Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

TEST_F(CpuTest, 8xy5_SetVFToOne_WhenVXIsGreaterThanVY){
    cpu.setRegist_V(0, 2);
    cpu.setRegist_V(1, 1);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.printRegist_V(0xF) << "but shouldn't be 0.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.printRegist_V(0xF) << "but should be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

TEST_F(CpuTest, 8xy5_VXHoldsCorrectValue_WhenVFIsVX_AndVXIsGreaterThanVY){
    secondNibble = 'f'; 
    cpu.setRegist_V(0xF, 20);
    cpu.setRegist_V(1, 15);    

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Incorrect value after subtracting VF - VY.  Ensure 8xy5 logic is correct when VF is VX";  
}

TEST_F(CpuTest, 8xy5_VXHoldsCorrectValue_WhenVXAndVYAreBothVF){
    secondNibble = 'f'; // to pass register VF into method 
    thirdNibble = 'f'; 
    cpu.setRegist_V(0xF, 60);

    cpu.subtractVYFromVX(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "The result should be 1 but wasn't.  Note that if VX is VF, VX's result after calling this method should be the flag (1 or 0).  Not the result from the VX - VY.  Check 8xy5's implementation to ensure this";   
}

// 8xy6 

TEST_F(CpuTest, 8xy6_VXHoldsCorrectValue_WhenShiftedRightOnce){
    cpu.setRegist_V(0, 128); 
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_EQ(cpu.getRegist_V(0), 64) << "VX should be 64.  But received incorrect value after shifting right. Check 8xy6's bitwise implementation"; 
}

TEST_F(CpuTest, 8xy6_VFHoldsShiftedOutBit_WhenVXShiftedRight){
    std::string errorMessage = "Check 0xF in 8xy6's and ensure VF correctly holds shifted out bit"; 
    cpu.setRegist_V(0, 15);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "VF shouldn't be 0. " << errorMessage; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "VF should be 1. " << errorMessage; 

    cpu.setRegist_V(0, 14);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, false); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "VF shouldn't be 1. " << errorMessage;  
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "VF should be 0. " << errorMessage; 
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
    std::string errorMessage = "Check 0xF in 8xy6's COSMAC implementation and ensure VF correctly holds shifted out bit"; 
    cpu.setRegist_V(0, 128);
    cpu.setRegist_V(1, 4);
    
    bool turnOnCosmacFlag = true; 
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "VF shouldn't be 1. " << errorMessage; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "VF should be 0. " << errorMessage; 

    cpu.setRegist_V(0, 128);
    cpu.setRegist_V(1, 15);
    
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "VF shouldn't be 0. " << errorMessage;  
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "VF should be 1. " << errorMessage; 
}

TEST_F(CpuTest, 8xy6_VFHoldsCorrectValue_WhenCosmacFlagIsOn_AndVYIsVF){
    thirdNibble = 'f'; 
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(0xF, 193);
    std::string errorMessage = "VF should be 1, when Cosmac flag is on.  Check Cosmac implementation in 8xy6's method";    

    bool turnOnCosmacFlag = true; 
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << errorMessage; 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << errorMessage; 
}

TEST_F(CpuTest, 8xy6_VXHoldsCorrectValue_WhenCosmacFlagIsOn_AndVYIsVF){
    thirdNibble = 'f'; 
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(0xF, 193);
    std::string errorMessage = "Result should be 96 after shifted right.  Check Cosmac implementation in 8xy6's method";    

    bool turnOnCosmacFlag = true; 
    cpu.shiftVXValueRight(secondNibble, thirdNibble, turnOnCosmacFlag); 
    EXPECT_EQ(cpu.getRegist_V(0), 96) << errorMessage; 
}

// 8xy7 

TEST_F(CpuTest, 8xy7_VXHoldsExpectedValue){
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 20);
    
    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 10) << "Result should be 88.  Check the substraction between the two operands or opcode implementation"; 
}

TEST_F(CpuTest, 8xy7_VXUnderflowsCorrectly_WhenVYIsLessThanVX){
    cpu.setRegist_V(0, 255);
    cpu.setRegist_V(1, 0);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 1) << "Register V1 should underflow to be 1.  ";  
}

TEST_F(CpuTest, 8xy7_VXHoldsCorrectValue_WhenVXAndVYAreSameValue){
    cpu.setRegist_V(0, 20);
    cpu.setRegist_V(1, 20);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0), 0) << "The result should be 0 but wasn't.  Check 8xy7's implementation when VX and VY are the same value ";  
}

TEST_F(CpuTest, 8xy7_VFHoldsCorrectValue_WhenVYAndVXAreSameValue){
    cpu.setRegist_V(0, 20);
    cpu.setRegist_V(1, 20);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register VF should be 1 but wasn't.  Check 8xy7's VF logic";  
}

TEST_F(CpuTest, 8xy7_SetVFToZero_WhenVYIsLessThanVX){
    cpu.setRegist_V(0, 255);
    cpu.setRegist_V(1, 1);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 1) << "Register 0xF shouldn't be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 0) << "Register 0xF should be 0. Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

TEST_F(CpuTest, 8xy7_SetVFToOne_WhenVYIsGreaterThanVX){
    cpu.setRegist_V(0, 1);
    cpu.setRegist_V(1, 255);    

    cpu.subtractVXFromVY(secondNibble, thirdNibble); 
    EXPECT_NE(cpu.getRegist_V(0xF), 0) << "Register 0xF was set to " << cpu.printRegist_V(0xF) << "but shouldn't be 0.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
    EXPECT_EQ(cpu.getRegist_V(0xF), 1) << "Register 0xF was set to " << cpu.printRegist_V(0xF) << "but should be 1.  Check 0xF in 8xy5's implementation to ensure underflow triggers carry flag"; 
}

// 9xy0 

TEST_F(CpuTest, 9xy0_ProgramCounterIncrementsByTwo_WhenVXNotEqualVY){
    std::string errorMessage = "Program Counter was expected to increment by 2, check conditional logic in 9xy0 instruction"; 
    cpu.setProgramCounter(cpu.getProgramCounterPointer(), 0x200); 
    
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 20);    

    cpu.skipInstructionIfVXNotEqualsVY(secondNibble, thirdNibble); 

    EXPECT_NE(cpu.getProgramCounter(), 0x200) << errorMessage; 
    EXPECT_EQ(cpu.getProgramCounter(), 0x202) << errorMessage; 
}

TEST_F(CpuTest, 9xy0_ProgramCounterRemainsSame_WhenVXEqualVY){
    cpu.setProgramCounter(cpu.getProgramCounterPointer(), 0x200); 
    
    cpu.setRegist_V(0, 10);
    cpu.setRegist_V(1, 10);    

    cpu.skipInstructionIfVXNotEqualsVY(secondNibble, thirdNibble); 

    EXPECT_EQ(cpu.getProgramCounter(), 0x200) << "Program Counter was expected to not increment by 2, check conditional logic in 9xy0 instruction"; 
}

// cxnn 

TEST_F(CpuTest, cxnn_VXHoldsCorrectValue_WhenInputValueIsZero){
    cpu.setRegist_V(0, 10);
    std::string inputValue = "0x0"; 

    cpu.bitwiseANDRandNumAndNN(secondNibble, inputValue); 

    EXPECT_EQ(cpu.getRegist_V(0), 0) << "Check the result is greater than or equal to zero, ensure that contraint is implemented "; 
}

TEST_F(CpuTest, cxnn_VXDoesntSurpassExpectedValue_WhenInputValueIsFifteen){
    cpu.setRegist_V(0, 10);
    std::string inputValue = "0xf"; 

    cpu.bitwiseANDRandNumAndNN(secondNibble, inputValue); 

    EXPECT_LE(cpu.getRegist_V(0), 15) << "Check the result is greater than the max expected value (15), check your bitwise AND implementation and ensure NN is handled correctly "; 
}

TEST_F(CpuTest, cxnn_VXLessThan255){
    cpu.setRegist_V(0, 10);
    std::string inputValue = "0xf"; 

    cpu.bitwiseANDRandNumAndNN(secondNibble, inputValue); 

    EXPECT_LE(cpu.getRegist_V(0), 255) << "Check the result is less than or equal to 255 (0xff), ensure that contraint is implemented "; 
}

TEST_F(CpuTest, cxnn_VXGreaterThanZero){
    cpu.setRegist_V(0, 10);
    std::string inputValue = "0xf"; 

    cpu.bitwiseANDRandNumAndNN(secondNibble, inputValue); 

    EXPECT_GE(cpu.getRegist_V(0), 0) << "Check the result is greater than or equal to zero, ensure that contraint is implemented "; 
}

class ConversionTestSuite : public testing::Test {
    protected:
        ConversionTestSuite(): cpu(debugger){}
        Debugger debugger; 
        Cpu cpu;

        std::string errorMessageForIncorrectInt = "Incorrect integer value was return from method, check method's conversion implementation"; 
        std::string errorMessageForIncorrectString = "Incorrect integer value was return from method, check method's conversion implementation"; 
}; 

TEST_F(ConversionTestSuite, convertCharToHex_ReturnsCorrectHexValue){
    char nibbleValue = '6'; 
    int result = cpu.convertCharToHex(nibbleValue); 

    EXPECT_EQ(result, 6) << errorMessageForIncorrectInt; 
}

TEST_F(ConversionTestSuite, convertCharToHex_ReturnsCorrectHexValue_WhenPassingInMaxValue){
    char nibbleValue = 'f'; 
    int result = cpu.convertCharToHex(nibbleValue); 

    EXPECT_EQ(result, 15) << errorMessageForIncorrectInt; 
}

TEST_F(ConversionTestSuite, convertCharToHex_ReturnsCorrectHexValue_WhenPassingInMinValue){
    char nibbleValue = '0'; 
    int result = cpu.convertCharToHex(nibbleValue); 

    EXPECT_EQ(result, 0) << errorMessageForIncorrectInt; 
}

TEST_F(ConversionTestSuite, convertIntToHexString_ReturnsCorrectHexString){
    std::string result = cpu.convertIntToHexString(648); 

    EXPECT_EQ(result, "288") << errorMessageForIncorrectString; 
}


class FetchInstructionsTestSuite : public testing::Test {
    protected:
        FetchInstructionsTestSuite(): cpu(debugger){}
        Debugger debugger; 
        Cpu cpu;
        Memory memory; 
}; 

TEST_F(FetchInstructionsTestSuite, fetchInstructions_FetchesCorrectOpcode){
    cpu.setProgramCounter(cpu.getProgramCounterPointer(), 200); 
    memory.systemMemory[cpu.getProgramCounter()] = 23; 
    memory.systemMemory[cpu.getProgramCounter()+1] = 64; 

    cpu.fetchInstructions(memory); 
    EXPECT_EQ(cpu.getCurrentInstruction(), "1740"); 
}

