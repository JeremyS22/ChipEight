#include <iostream> 
#include <cstdint> 
#include <string> 

uint8_t memory[4096] = {

// unsigned char memory[4096] = {

    // 0x050 =  
}; 

// have stack big enough for 16 2 byte address, may make bigger later 
uint16_t stack[16]; 

uint8_t delayTimer; 

uint8_t soundTimer; 

uint16_t programCounter; 

uint8_t stackPointer; 

// points at locations in memory 
uint16_t regist_I; 

uint8_t regist_V0; 
uint8_t regist_V1; 
uint8_t regist_V2; 
uint8_t regist_V3; 
uint8_t regist_V4; 
uint8_t regist_V5; 
uint8_t regist_V6; 
uint8_t regist_V7; 
uint8_t regist_V8; 
uint8_t regist_V9;
uint8_t regist_VA; 
uint8_t regist_VB; 
uint8_t regist_VC; 
uint8_t regist_VD; 
uint8_t regist_VE; 
uint8_t regist_VF; 

// VF is used as a carry flag register by some instructions, set to either 1 or 0 
// 0x000 to 0x1FF 

// Reserved for interpreter 

// TODO: Place fonts here (Preferably 050 - 09F) 


// 0x200 to 0xFFF Chip 8 program/data space 

// set this as a private member? 
std::string currentInstruction; 

uint8_t fontData[] = {

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

void loadDataIntoMemory(uint8_t fontDataArray[], uint8_t memory[]); 

void loadRomIntoMemory(uint8_t memory[], std::string romFileLocation, uint16_t* programCounter); 

void setProgramCounter(uint16_t * programCounter, int value); 

void incrementProgramCounter(uint16_t * programCounter, int value); 

// including this just in case, may delete this later 
void decrementProgramCounter(uint16_t * programCounter, int value); 

uint16_t getProgramCounter (); 

void setCurrentInstruction (std::string Instruction); 

std::string getCurrentInstruction (); 

void findRegisterOfNibble(char nibble, std::string lastTwoNibbles); 

std::string getLastTwoNibbles (std::string currentInstruction); 

std::string getLastThreeNibbles (std::string currentInstruction); 

// 1nnn 
void jumpToAddress(std::string address); 

// 6xnn 
void setValueInRegisterX (char secondNibble, std::string value); 

// annn 
void loadAddressInRegisterI(std::string address); 

void fetchInstructions(uint8_t memory[]); 

void decodeAndExecuteInstructions(std::string currentInstruction); 
 

// Use for debugging ------------------------------------------------------------------------------- 

void printMemory(uint8_t memory[]); 

// TODO: Expand this to include other registers 
void printCurrentInstruction(std::string Instruction); 

