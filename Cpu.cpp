#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream> 
#include <iomanip> 
#include <bitset> 

#include "Screen.h" 
#include "Memory.h" 

using namespace std; 

void setProgramCounter(uint16_t * programCounter, int value){
    *programCounter = value;  
}

void incrementProgramCounter(uint16_t * programCounter, int value){
    *programCounter += value;  
}

// including this just in case, may delete this later 
void decrementProgramCounter(uint16_t * programCounter, int value){
    *programCounter -= value;  
}

uint16_t getProgramCounter (){
    return programCounter; 
}

void setCurrentInstruction (string Instruction) {
    currentInstruction = Instruction; 
}

string getCurrentInstruction (){
    return currentInstruction; 
}

string getLastTwoNibbles (string currentInstruction){
    stringstream nibbleParser;

    // TODO: Check if we might have to delete the first two 0's, since Vx registers are 8 bits, not 16 
    nibbleParser << currentInstruction[2] << currentInstruction[3]; 
    string lastTwoNibbles = nibbleParser.str(); 

    cout << lastTwoNibbles << endl; 

    return lastTwoNibbles; 

}

string getLastThreeNibbles (string currentInstruction){
    stringstream nibbleParser;

    nibbleParser << "0" << currentInstruction[1] << currentInstruction[2] << currentInstruction[3]; 
        string lastThreeNibbles = nibbleParser.str(); 

    cout << lastThreeNibbles << endl; 

    return lastThreeNibbles; 

}

// 00e0
void clearScreenInstruction(Screen screen){
    SDL_RenderClear(screen.renderer); 
    SDL_RenderPresent(screen.renderer); 
}

// 1nnn 
void jumpToAddress(string address){
    setProgramCounter(&programCounter, stoi(address, nullptr, 16));   
}


// 6xnn 
void setValueInRegisterVX (char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int lastTwoNibbles = stoi(value, nullptr, 16); 
    regist_V[X] = lastTwoNibbles; 
}

// 7xnn
void addValueToRegisterVX (char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int originalVXValue = regist_V[X]; 
    regist_V[X] = originalVXValue + stoi(value, nullptr, 16);     
}

// annn 
void loadAddressInRegisterI(string address){
    // add to debugger 
    cout << address << "Address being stored in Register I " << hex << setw(2) << setfill('0') << stoi(address, nullptr, 16) << endl; 
    regist_I = stoi(address, nullptr, 16); 
}

// dxyn 
void drawSpriteAtVXAndVY(char secondNibble, char thirdNibble, char fourthNibble, Screen screen, 
                            Memory memory ){

    // secondNibble = VX, thirdNibble = VY 
    int X = convertCharToHex(secondNibble);  
    int Y = convertCharToHex(thirdNibble); 
    int spriteHeight = convertCharToHex(fourthNibble); 

    int coordinateX = regist_V[X];
    int coordinateY = regist_V[Y];  

    

    SDL_RenderSetScale(screen.renderer, screen.getScalingMultipiler(), screen.getScalingMultipiler());    
    uint16_t spriteDataAddress = regist_I; 
    
    for(int i = 0; i < spriteHeight; i++){
        uint16_t binaryVal = memory.memory[spriteDataAddress]; 
        bitset<16> binaryValue (binaryVal); 

        // TODO: add to debugger 
        cout << "Binary value from memory address of register I " << binaryValue << endl; 

        for(int j = 7; j >= 0; j--){
            if(binaryValue[j] == 1){
                SDL_SetRenderDrawColor(screen.renderer, 255, 0, 255, 255); 
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

void fetchInstructions(Memory memory){

    stringstream instructionString;
    instructionString << hex << setw(2) << setfill('0') << (int)memory.memory[getProgramCounter()];
    instructionString << hex << setw(2) << setfill('0') << (int)memory.memory[getProgramCounter()+1];

    setCurrentInstruction(instructionString.str()); 

    incrementProgramCounter(&programCounter, 2); 
    cout << getProgramCounter() << endl; 

    debug_printCurrentInstruction(getCurrentInstruction());  
}

void decodeAndExecuteInstructions(string currentInstruction, Screen screen, Memory memory){
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
                jumpToAddress(getLastThreeNibbles(currentInstruction));  // call function 
                break;
            case '2': 
                getCurrentInstruction(); // call the subroutine call function 2nnn 
                break;
            case '3': 
                getCurrentInstruction(); // call skip next instruction 3xkk 
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
                break;
            default: 
                cout << "Status: (CPU) Error, Opcode not found " << endl; 
        }
}
 
int convertCharToHex(char Value){
        stringstream hexString; 
        hexString << "0x" << Value;     
    return stoi(hexString.str(), nullptr, 16);    
}

// Use for debugging ------------------------------------------------------------------------------- 

// TODO: Expand this to include other registers 
void debug_printCurrentInstruction(string Instruction){
    cout << "Current Instruction: " << Instruction << endl; 

}