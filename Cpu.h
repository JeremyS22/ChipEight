#include <iostream> 
#include <cstdint> 
#include <string> 

#include "Screen.h" 

// 0x000 to 0x1FF are reserved for interpreter 
// font data at 0x050 - 0x09F  
// 0x200 to 0xFFF Chip 8 program/data space 
uint8_t memory[4096]; 

// have stack big enough for 16 2 byte address, may make bigger later 
uint16_t stack[16]; 

uint8_t delayTimer; 

uint8_t soundTimer; 

uint16_t programCounter; 

uint8_t stackPointer; 

// TODO: set constructor to intialize this to 0 
// points at locations in memory 
uint16_t regist_I = 0; 

uint8_t regist_V[16]; 
 
// VF is used as a carry flag register by some instructions, set to either 1 or 0 

// TODO: set this as a private member 
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

// 00e0 
void clearScreenInstruction(Screen screen);  

// 1nnn 
void jumpToAddress(std::string address); 

// 6xnn 
void setValueInRegisterVX (char secondNibble, std::string value); 

// annn 
void loadAddressInRegisterI(std::string address); 

// dxyn 
void drawSpriteAtVXAndVY(char secondNibble, char thirdNibble, char fourthNibble, Screen screen, 
                            uint8_t memory[]); 

void fetchInstructions(uint8_t memory[]); 

void decodeAndExecuteInstructions(std::string currentInstruction, Screen screen, uint8_t memory[]); 

int convertCharToHex(char Value); 
 

// Use for debugging ------------------------------------------------------------------------------- 

void debug_printMemory(uint8_t memory[]); 

// TODO: Expand this to include other registers 
void debug_printCurrentInstruction(std::string Instruction); 

