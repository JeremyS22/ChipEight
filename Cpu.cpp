#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream> 
#include <iomanip> 
#include <bitset> 
#include <stack> 

#include "Cpu.h"
#include "Screen.h" 
#include "Memory.h" 

using namespace std; 

// for run older ROMs from 1980s and 1970s 
bool COSMAC_VIP_FLAG_IS_ON = false; 

Cpu::Cpu(){
    regist_I = 0; 
}

void Cpu::setProgramCounter(uint16_t * programCounter, int value){
    *programCounter = value;  
}

void Cpu::incrementProgramCounter(uint16_t * programCounter, int value){
    *programCounter += value;  
}

// including this just in case, may delete this later 
void Cpu::decrementProgramCounter(uint16_t * programCounter, int value){
    *programCounter -= value;  
}

uint16_t Cpu::getProgramCounter(){
    return programCounter; 
}

uint16_t& Cpu::getProgramCounterReference(){
    return programCounter; 
}

void Cpu::setCurrentInstruction (string Instruction) {
    currentInstruction = Instruction; 
}

string Cpu::getCurrentInstruction(){
    return currentInstruction; 
}

void Cpu::pushProgramCounterOnStack(Memory memory){
    memory.systemStack.push(getProgramCounter());  
}

string Cpu::getLastTwoNibbles (string currentInstruction){
    stringstream nibbleParser;

    // TODO: Check if we might have to delete the first two 0's, since Vx registers are 8 bits, not 16 
    nibbleParser << currentInstruction[2] << currentInstruction[3]; 
    string lastTwoNibbles = nibbleParser.str(); 

    cout << lastTwoNibbles << endl; 

    return lastTwoNibbles; 

}

string Cpu::getLastThreeNibbles (string currentInstruction){
    stringstream nibbleParser;

    nibbleParser << "0" << currentInstruction[1] << currentInstruction[2] << currentInstruction[3]; 
    string lastThreeNibbles = nibbleParser.str(); 

    cout << lastThreeNibbles << endl; 

    return lastThreeNibbles; 

}

// 0nnn
/*Instruction to execute machine language routine (Not implementing)*/

// 00e0
void Cpu::clearScreenInstruction(Screen screen){
    SDL_RenderClear(screen.renderer); 
    SDL_RenderPresent(screen.renderer); 
}

// 1nnn 
void Cpu::jumpToAddress(string address){
    setProgramCounter(&programCounter, stoi(address, nullptr, 16));   
}

// 2nnn
void Cpu::putAddressOnStack(string address, Memory memory){
    pushProgramCounterOnStack(memory); 
    setProgramCounter(&programCounter, stoi(address, nullptr, 16)); 
    
    cout << "Previous Program Counter on stack, new address for Program Counter " << getProgramCounter() << endl; 
    debug_printCurrentInstruction(getCurrentInstruction()); 
    cout << "\n" << endl;  
}

// 3xnn 
void Cpu::skipInstructionIfVXEqualsNN(char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int lastTwoNibbles = stoi(value, nullptr, 16); 
    if(regist_V[X] == lastTwoNibbles){
        incrementProgramCounter(&programCounter, 2); 
        cout << "Program Counter " << getProgramCounter() << endl; 
    }
}

// 6xnn 
void Cpu::setValueInRegisterVX (char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int lastTwoNibbles = stoi(value, nullptr, 16); 
    regist_V[X] = lastTwoNibbles; 
}

// 7xnn
void Cpu::addValueToRegisterVX (char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int originalVXValue = regist_V[X]; 
    regist_V[X] = originalVXValue + stoi(value, nullptr, 16);     
}

// annn 
void Cpu::loadAddressInRegisterI(string address){
    // add to debugger 
    cout << address << "Address being stored in Register I " << hex << setw(2) << setfill('0') << stoi(address, nullptr, 16) << endl; 
    regist_I = stoi(address, nullptr, 16); 
}

// dxyn 
void Cpu::drawSpriteAtVXAndVY(char secondNibble, char thirdNibble, char fourthNibble, Screen screen, 
                            Memory memory){

    // secondNibble = VX, thirdNibble = VY 
    int X = convertCharToHex(secondNibble);  
    int Y = convertCharToHex(thirdNibble); 
    int spriteHeight = convertCharToHex(fourthNibble); 

    int coordinateX = regist_V[X];
    int coordinateY = regist_V[Y];  

    

    SDL_RenderSetScale(screen.renderer, screen.getScalingMultipiler(), screen.getScalingMultipiler());    
    uint16_t spriteDataAddress = regist_I; 
    
    for(int i = 0; i < spriteHeight; i++){
        uint16_t binaryVal = memory.systemMemory[spriteDataAddress]; 
        bitset<16> binaryValue (binaryVal); 

        // TODO: add to debugger 
        cout << "Binary value from memory address of register I " << binaryValue << endl; 

        for(int j = 7; j >= 0; j--){
            if(binaryValue[j] == 1){
                // TODO: add custom renderer color 
                SDL_SetRenderDrawColor(screen.renderer, 179, 254, 238, 1); 
                SDL_RenderDrawPoint(screen.renderer, coordinateX, coordinateY);    
            }
            screen.setPixelStatus(coordinateX, coordinateY, binaryValue[j]); 
            coordinateX++; 
        }
        spriteDataAddress++; 
        coordinateY++; 
        // resets X coordinate for drawing the next line 
        coordinateX = regist_V[X];    
    }
    SDL_RenderPresent(screen.renderer); 
}

// fx55 
void Cpu::storeRegistersToMemory(char secondNibble, Memory memory, bool COSMAC_VIP_FLAG_IS_ON){
    int X = convertCharToHex(secondNibble); 
    uint16_t tempAddress = regist_I; 
    for (int i = 0; i < X; i++){
        memory.systemMemory[tempAddress] = regist_V[i]; 
        tempAddress++; 
    }

    if (COSMAC_VIP_FLAG_IS_ON == true){
        regist_I = tempAddress; 
    }
} 

void Cpu::fetchInstructions(Memory memory){

    stringstream instructionString;
    instructionString << hex << setw(2) << setfill('0') << (int)memory.systemMemory[getProgramCounter()];
    instructionString << hex << setw(2) << setfill('0') << (int)memory.systemMemory[getProgramCounter()+1];

    setCurrentInstruction(instructionString.str()); 

    incrementProgramCounter(&programCounter, 2); 
    cout << "Program Counter " << getProgramCounter() << endl; 

    debug_printCurrentInstruction(getCurrentInstruction());  
}

void Cpu::decodeAndExecuteInstructions(string currentInstruction, Screen screen, Memory memory){
    char firstNibble = currentInstruction[0]; 
    char secondNibble = currentInstruction[1]; 
    char thirdNibble = currentInstruction[2]; 
    char fourthNibble = currentInstruction[3]; 


        switch(firstNibble){
            case '0': 
                getCurrentInstruction(); // call function 
                switch (fourthNibble){
                    case '0':
                    // call clear screen function 00e0 
                    clearScreenInstruction(screen); 
                    break; 
                    case 'e':
                    // call return function 00ee
                    break; 
                } 
                break;
            case '1':
                // call jump address function 1nnn  
                // Note: n = the 12 bit address  
                jumpToAddress(getLastThreeNibbles(currentInstruction));  
                break;
            case '2': 
                // call the subroutine call function 2nnn 
                putAddressOnStack(getLastThreeNibbles(getCurrentInstruction()), memory); 
                break;
            case '3': 
                // call skip next instruction 3xkk 
                skipInstructionIfVXEqualsNN(secondNibble, getLastTwoNibbles(getCurrentInstruction())); 
                break;
            case '4': 
                getCurrentInstruction(); // call function 
                break;
            case '5': 
                getCurrentInstruction(); // call function 
                break;
            case '6': 
                // 600c 
                // call set register Vx function 
                setValueInRegisterVX(secondNibble, getLastTwoNibbles(currentInstruction)); 
                break;
            case '7': 
                // call add value to register Vx function  
                addValueToRegisterVX(secondNibble, getLastTwoNibbles(currentInstruction)); 
                break;
            case '8': 
                getCurrentInstruction(); // call function 
                break;
            case '9': 
                getCurrentInstruction(); // call function 
                break;
            case 'a': 
                // call set index to register I function  
                loadAddressInRegisterI(getLastThreeNibbles(currentInstruction));  
                break; 
            case 'b': 
                getCurrentInstruction(); // call function 
                break;
            case 'c': 
                getCurrentInstruction(); // call function 
                break;
            case 'd': 
                // call display/draw fucntion  
                drawSpriteAtVXAndVY(secondNibble, thirdNibble, fourthNibble, screen, memory);  
                break;
            case 'e':       
                getCurrentInstruction(); // call function 
                break;
            case 'f': 
                getCurrentInstruction(); // call function 
                switch (thirdNibble){
                    case '1':
                        // call fx1e  
                        getCurrentInstruction(); // call function 
                        break; 
                    case '3':
                        // call fx33 
                        getCurrentInstruction(); // call function 
                    break; 
                    case '5':
                        // call fx55  
                        storeRegistersToMemory(secondNibble, memory, COSMAC_VIP_FLAG_IS_ON); 
                    break; 
                    case '6':
                        // call fx65 
                        getCurrentInstruction(); // call function 
                    break; 
                } 
                break;
                break;
            default: 
                cout << "Status: (CPU) Error, Opcode not found " << endl; 
        }
}
 
int Cpu::convertCharToHex(char Value){
        stringstream hexString; 
        hexString << "0x" << Value;     
    return stoi(hexString.str(), nullptr, 16);    
}

// Use for debugging ------------------------------------------------------------------------------- 

// TODO: Expand this to include other registers 
void Cpu::debug_printCurrentInstruction(string Instruction){
    cout << "Current Instruction: " << Instruction << endl; 

}