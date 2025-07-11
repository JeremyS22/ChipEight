#ifndef CPU_H
#define CPU_H  

#include <iostream> 
#include <cstdint> 
#include <string> 

#include "Screen.h" 
#include "Memory.h"  
class Memory;

// 0x000 to 0x1FF are reserved for interpreter 
// font data at 0x050 - 0x09F  
// 0x200 to 0xFFF Chip 8 program/data space 

extern bool COSMAC_VIP_FLAG_IS_ON; 

class Cpu {

    private: 

        uint16_t programCounter; 

    public:
        
        uint8_t delayTimer; 

        uint8_t soundTimer; 

        uint8_t stackPointer; 

        // TODO: set constructor to intialize this to 0 
        // points at locations in memory 
        uint16_t regist_I; 

        uint8_t regist_V[16]; 

        // TODO: set this as a private member 
        std::string currentInstruction; 


        uint8_t fontData[80] =  {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F 

    }; 

    Cpu(); 
    ~Cpu(){}
    
void setProgramCounter(uint16_t * programCounter, int value); 

void incrementProgramCounter(uint16_t * programCounter, int value); 

// including this just in case, may delete this later 
void decrementProgramCounter(uint16_t * programCounter, int value); 

uint16_t getProgramCounter(); 

uint16_t& getProgramCounterReference(); 

uint16_t* getProgramCounterPointer(); 

void setCurrentInstruction (std::string Instruction); 

std::string getCurrentInstruction(); 

void pushProgramCounterOnStack(Memory& memory); 

void popProgramCounterOffStack(Memory& memory); 

std::string getLastTwoNibbles(std::string currentInstruction); 

std::string getLastThreeNibbles(std::string currentInstruction); 

// 0nnn (Not implementing)

// 00e0 
void clearScreenInstruction(Screen screen);  

//00ee 
void returnToAddressFromStack(Memory& memory); 

// 1nnn 
void jumpToAddress(std::string address); 

// 2nnn
void putAddressOnStack(std::string address, Memory& memory); 

// 3xnn 
void skipInstructionIfVXEqualsNN(char secondNibble, std::string value); 

// 4xnn 
void skipInstructionIfVXNotEqualsNN(char secondNibble, std::string value); 

// 5xnn 
void skipInstructionIfVXEqualsVY(char secondNibble, char thirdNibble); 

// 6xnn 
void setValueInRegisterVX(char secondNibble, std::string value); 

// 7xnn
void addValueToRegisterVX(char secondNibble, std::string value); 

// 8xy0 
void setVXToValueOfVY(int secondNibble, int thirdNibble, bool COSMAC_VIP_FLAG_IS_ON); 

// 9xyn 
void skipInstructionIfVXNotEqualsVY(char secondNibble, char thirdNibble); 

// annn 
void loadAddressInRegisterI(std::string address); 

// dxyn 
void drawSpriteAtVXAndVY(char secondNibble, char thirdNibble, char fourthNibble, Screen screen, 
                            Memory memory); 

// fx55 
void storeRegistersToMemory(char secondNibble, Memory memory, bool COSMAC_VIP_FLAG_IS_ON); 

void fetchInstructions(Memory memory); 

void decodeAndExecuteInstructions(std::string currentInstruction, Screen screen, Memory& memory); 

int convertCharToHex(char Value); 
 

// Use for debugging ------------------------------------------------------------------------------- 

// TODO: Expand this to include other registers 
void debug_printCurrentInstruction(std::string Instruction); 

}; 

#endif 