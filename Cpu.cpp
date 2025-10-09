#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream> 
#include <iomanip> 
#include <bitset> 
#include <stack> 
#include <cstring> 
#include <chrono> 
#include <typeinfo>

#include "Cpu.h"

using namespace std; 

// for running older ROMs from 1980s and 1970s 
bool COSMAC_VIP_FLAG_IS_ON = true; 

// injecting this debugger instance to avoid passing it 
//      in the majority of function parameters in this class 
Cpu::Cpu(Debugger& debugger) : debugger(debugger){ 
    programCounter = 0; 
    regist_I = 0; 
    memset(regist_V, 0, sizeof(regist_V)); 
    delayTimer = 0; 
    soundTimer = 0; 
    delayTimerThreadIsRunning.store(false); 
    newDelayTimerThreadWaiting.store(false); 
}; 

void Cpu::setProgramCounter(uint16_t* programCounter, int value){
    *programCounter = value;  
}

void Cpu::incrementProgramCounter(uint16_t* programCounter, int value){
    *programCounter += value;  
}

// including this just in case, may delete this later 
void Cpu::decrementProgramCounter(uint16_t* programCounter, int value){
    *programCounter -= value;  
}

uint16_t Cpu::getProgramCounter(){
    return programCounter; 
}

uint16_t& Cpu::getProgramCounterReference(){
    return programCounter; 
}

// getter for &programCounter 
uint16_t* Cpu::getProgramCounterPointer(){
    return &programCounter; 
}

void Cpu::setCurrentInstruction (string Instruction) {
    currentInstruction = Instruction; 
}

string Cpu::getCurrentInstruction(){
    return currentInstruction; 
}

void Cpu::setRegist_V(int name, uint8_t value){
    regist_V[name] = value; 
}

uint8_t Cpu::getRegist_V(int name){
    return regist_V[name];  
}

void Cpu::setRegist_I(uint16_t address){
    regist_I = address; 
} 

uint16_t Cpu::getRegist_I(){
    return regist_I; 
} 

void Cpu::setDelayTimer(uint8_t time){
    delayTimer = time; 
}

uint8_t Cpu::getDelayTimer(){
    return delayTimer; 
}

int Cpu::printRegist_V(int name){
    return (int)regist_V[name]; 
}

void Cpu::pushProgramCounterOnStack(Memory& memory){
    memory.systemStack.push(getProgramCounter());  
    memory.setStackPointer(memory.systemStack.top()); 
    
    cout << "STACK SIZE " <<  memory.systemStack.size() << " top of stack " << memory.getStackPointer() << endl; 
}

void Cpu::popProgramCounterOffStack(Memory& memory){
    cout << "STACK SIZE (before pop) " <<  memory.systemStack.size() << endl; 
    memory.systemStack.pop();  
    
    if(!memory.systemStack.empty()){
        memory.setStackPointer(memory.systemStack.top()); 
        cout << "Top of stack " << memory.getStackPointer() << endl; 
    }
    else {
        // to avoid setting stack pointer to random place in memory when the stack is empty 
        memory.setStackPointer(0); 
        cout << "Stack empty, stack pointer set to 0 " << endl; 
    } 
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

bool Cpu::runDelayTimer(){
    if(delayTimerThreadIsRunning.load() == true){
        newDelayTimerThreadWaiting.store(true); 
        while(delayTimerThreadIsRunning.load() == true){
            this_thread::sleep_for(chrono::nanoseconds(8333333)); 
        } 
        newDelayTimerThreadWaiting.store(false); 
    }

    delayTimerThreadIsRunning.store(true); 
    cout << "    Delay Timer thread started and In threaded function " << endl; 
    uint8_t localDelayTimer = getDelayTimer(); 

    while(localDelayTimer > 0){
        if(newDelayTimerThreadWaiting.load() == true){
            delayTimerThreadIsRunning.store(false); 
            cout << "    Aborting delay timer in current thread!  New fx15 opcode called in another thread " << endl; 
            return true; 
        }

        auto startOfComputeClock = chrono::steady_clock::now(); 
        localDelayTimer-=1; 
        setDelayTimer(localDelayTimer); 
        debugger.outputDelayTimerToDebugger(getDelayTimer()); 
        auto endOfComputeClock = chrono::steady_clock::now(); 

        chrono::duration<double> timeElasped = endOfComputeClock - startOfComputeClock; 
        int timeToSleep = 16666666 - (int)timeElasped.count(); 
        this_thread::sleep_for(chrono::nanoseconds(timeToSleep)); 
    }
    cout << "\n    Exiting threaded function" << endl; 

    delayTimerThreadIsRunning.store(false); 
    return true; 
}

future<bool>& Cpu::getFuture(){
    return delayTimerFuture; 
}

bool Cpu::checkDelayTimerThreadIsRunning(){
    return delayTimerThreadIsRunning.load(); 
}

// 0nnn
/*Instruction to execute machine language routine (Not implementing)*/

// 00e0
void Cpu::clearScreenInstruction(Screen& screen, Cpu& cpu){
    SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 0); 
    SDL_RenderClear(screen.renderer); 
    screen.turnOffAllPixels(cpu); 
    SDL_RenderPresent(screen.renderer); 
}

//00ee 
void Cpu::returnToAddressFromStack(Memory& memory){

    uint16_t address = memory.getStackPointer(); 

    cout << "PROGRAM COUNTER FROM STACK "<< address << endl; 
    setProgramCounter(getProgramCounterPointer(), address); 
    popProgramCounterOffStack(memory); 

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputStackToDebugger(memory); 
    }
}

// 1nnn 
void Cpu::jumpToAddress(string address){
    setProgramCounter(getProgramCounterPointer(), stoi(address, nullptr, 16));   
}

// 2nnn
void Cpu::putAddressOnStack(string address, Memory& memory){
    pushProgramCounterOnStack(memory); 
    setProgramCounter(getProgramCounterPointer(), stoi(address, nullptr, 16)); 

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputStackToDebugger(memory);
    } 
}

// 3xnn 
void Cpu::skipInstructionIfVXEqualsNN(char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int lastTwoNibbles = stoi(value, nullptr, 16); 
    if(regist_V[X] == lastTwoNibbles){
        incrementProgramCounter(getProgramCounterPointer(), 2); 
        cout << "TRUE, VX == NN Incremented Program Counter to " << getProgramCounter() << endl; 
    }
}

// 4xnn 
void Cpu::skipInstructionIfVXNotEqualsNN(char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int lastTwoNibbles = stoi(value, nullptr, 16); 
    if(regist_V[X] != lastTwoNibbles){
        incrementProgramCounter(getProgramCounterPointer(), 2); 
        cout << "TRUE, VX != NN Incremented Program Counter to " << getProgramCounter() << endl; 
    }
}

// 5xy0 
void Cpu::skipInstructionIfVXEqualsVY(char secondNibble, char thirdNibble){
    int X = convertCharToHex(secondNibble); 
    int Y = convertCharToHex(thirdNibble); 
    
    if(regist_V[X] == regist_V[Y]){
        incrementProgramCounter(getProgramCounterPointer(), 2); 
        cout << "TRUE, VX == VY Incremented Program Counter to " << getProgramCounter() << endl; 
    }
}

// 6xnn 
void Cpu::setValueInRegisterVX (char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int lastTwoNibbles = stoi(value, nullptr, 16); 
    regist_V[X] = lastTwoNibbles; 

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
    }
}

// 7xnn
void Cpu::addValueToRegisterVX (char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    int originalVXValue = regist_V[X]; 
    regist_V[X] = originalVXValue + stoi(value, nullptr, 16);     

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
    }
}

// 8xy0 
void Cpu::setVXToValueOfVY(char secondNibble, char thirdNibble){
    int X = convertCharToHex(secondNibble); 
    int Y = convertCharToHex(thirdNibble); 
    regist_V[X] = regist_V[Y]; 

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
    }
}

// 8xy1 
void Cpu::bitwiseOrVXAndVY(char secondNibble, char thirdNibble, bool COSMAC_VIP_FLAG_IS_ON){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 
    regist_V[X] |= regist_V[Y]; 
    cout << "AFTER BITWISE OR " << printRegist_V(X) << endl; 

    if(COSMAC_VIP_FLAG_IS_ON == true){
        regist_V[0xF] = 0;  
        if(debugger.getDebuggerIsOn() == true){
            debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF); 
        }
    }

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
    }
}

// 8xy2  
void Cpu::bitwiseAndVXAndVY(char secondNibble, char thirdNibble, bool COSMAC_VIP_FLAG_IS_ON){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 
    regist_V[X] &= regist_V[Y]; 
    cout << "AFTER BITWISE AND " << printRegist_V(X) << endl; 

    if(COSMAC_VIP_FLAG_IS_ON == true){
        regist_V[0xF] = 0;  
        if(debugger.getDebuggerIsOn() == true){
            debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF); 
        }
    }


    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
    }
}

// 8xy3   
void Cpu::bitwiseExclusiveOrVXAndVY(char secondNibble, char thirdNibble, bool COSMAC_VIP_FLAG_IS_ON){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 
    regist_V[X] ^= regist_V[Y]; 
    cout << "AFTER BITWISE XOR " << printRegist_V(X)<< endl; 

    if(COSMAC_VIP_FLAG_IS_ON == true){
        regist_V[0xF] = 0;  
        if(debugger.getDebuggerIsOn() == true){
            debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF); 
        }
    }

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
    }
}

// 8xy4    
void Cpu::addVXToVY(char secondNibble, char thirdNibble){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 
    int result = regist_V[X] + regist_V[Y]; 
    if(result > 255){
        // subtracting 1 to compensate for uint8_t's range starting on 0 [0,255] 
        regist_V[X] = (result % 255) - 1; 

        // set carry flag in register VF 
        regist_V[0xF] = 1; 
    }
    else {
        regist_V[X] = result; 
        regist_V[0xF] = 0; 
    }

    cout << "AFTER adding VX and VY " << printRegist_V(X) << "also register VF is " << printRegist_V(0xF) << endl; 

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
        debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF); 
    }
}

// 8xy5     
void Cpu::subtractVYFromVX(char secondNibble, char thirdNibble){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 
    
    uint8_t result = regist_V[X] - regist_V[Y]; 

    if(regist_V[X] >= regist_V[Y]){
        regist_V[0xF] = 1; 

        if(debugger.getDebuggerIsOn() == true){
            debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF); 
        }

        cout << "VX > VY! Register VF is " << printRegist_V(0xF)<< endl; 
    }
    else {
        regist_V[0xF] = 0; 

        if(debugger.getDebuggerIsOn() == true){
            debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF); 
        }

        cout << "VX < VY! Register VF is " << printRegist_V(0xF) << endl; 
    }

    if(X != 0xF){
        regist_V[X] = result; 
    }

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(X), X); 
    }

    cout << "AFTER VX - VY! " << printRegist_V(X) << " also register VF is " << printRegist_V(0xF) << endl; 

}

// 8xy6 
void Cpu::shiftVXValueRight(char secondNibble, char thirdNibble, bool COSMAC_VIP_FLAG_IS_ON){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 
    
    if (COSMAC_VIP_FLAG_IS_ON == true){
        regist_V[X] = regist_V[Y];   

        if(debugger.getDebuggerIsOn() == true){
            debugger.outputRegistersToDebugger(getRegist_V(X), X);   
        }
    }

    bitset<8> binaryValueOfVX (regist_V[X]); 
    regist_V[0xF] = binaryValueOfVX[0];     

    if(X != 0xF){
        regist_V[X] >>= 1;     
    }

    if(debugger.getDebuggerIsOn() == true){
        if(X != 0xF){
            debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF);   
        } 
        debugger.outputRegistersToDebugger(getRegist_V(X), X);   
    }

}

// 8xy7      
void Cpu::subtractVXFromVY(char secondNibble, char thirdNibble){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 

    uint8_t result = regist_V[Y] - regist_V[X];  

    if(regist_V[Y] >= regist_V[X]){
        regist_V[0xF] = 1; 
        
        cout << "VX > VY! Register VF is " << printRegist_V(0xF)<< endl; 
    }
    else {
        regist_V[0xF] = 0; 
        cout << "VX < VY! Register VF is " << printRegist_V(0xF)<< endl; 
    }
    if(X != 0xF){
        regist_V[X] = result; 
    }

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF); 
        debugger.outputRegistersToDebugger(getRegist_V(X), X);   
    }
    cout << "AFTER VX - VY! " << printRegist_V(X) << "also register VF is " << printRegist_V(0xF)<< endl; 
}

// 8xyE 
void Cpu::shiftVXValueLeft(char secondNibble, char thirdNibble, bool COSMAC_VIP_FLAG_IS_ON){
    uint8_t X = convertCharToHex(secondNibble); 
    uint8_t Y = convertCharToHex(thirdNibble); 
    
    if (COSMAC_VIP_FLAG_IS_ON == true){
        regist_V[X] = regist_V[Y]; 

        if(debugger.getDebuggerIsOn() == true){  
            debugger.outputRegistersToDebugger(getRegist_V(X), X);   
        }
    }

    bitset<8> binaryValueOfVX (regist_V[X]); 
    regist_V[0xF] = binaryValueOfVX[7]; 
    if(X != 0xF){
        regist_V[X] <<= 1;
    }
     
    if(debugger.getDebuggerIsOn() == true){
        if(X != 0xF){
            debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF);   
        } 
        debugger.outputRegistersToDebugger(getRegist_V(X), X);   
    }
}

// 9xy0 
void Cpu::skipInstructionIfVXNotEqualsVY(char secondNibble, char thirdNibble){
    int X = convertCharToHex(secondNibble); 
    int Y = convertCharToHex(thirdNibble); 
    if(regist_V[X] != regist_V[Y]){
        incrementProgramCounter(getProgramCounterPointer(), 2); 
        cout << "TRUE, VX != VY Incremented Program Counter to " << getProgramCounter() << endl; 
    }
}

// annn 
void Cpu::loadAddressInRegisterI(string address){
    // add to debugger 
    cout << address << "Address being stored in Register I " << hex << setw(2) << setfill('0') << stoi(address, nullptr, 16) << endl; 
    regist_I = stoi(address, nullptr, 16); 
    if(debugger.getDebuggerIsOn() == true){
        debugger.outputIndexRegisterToDebugger(address); 
    }
}

// bnnn/bxnn 

void Cpu::jumpToAddressWithVXOffset(char secondNibble, string address){
    int addressWithVXOffset = regist_V[0] + stoi(address, nullptr, 16); 
    setProgramCounter(getProgramCounterPointer(), addressWithVXOffset);   
}   

// cxnn 

void Cpu::bitwiseANDRandNumAndNN(char secondNibble, string value){
    int X = convertCharToHex(secondNibble); 
    uint8_t lastTwoNibbles = stoi(value, nullptr, 16); 

    srand(time(0)); 
    uint8_t randomNum = rand() % 255; 

    regist_V[X] = lastTwoNibbles & randomNum; 

    cout << "Random Number is " << (int)randomNum << " NN is " << (int)lastTwoNibbles << endl; 
}

// dxyn 
void Cpu::drawSpriteAtVXAndVY(char secondNibble, char thirdNibble, char fourthNibble, Screen& screen, 
                            Memory memory, Cpu& cpu){
                                
    // secondNibble is VX, thirdNibble is VY 
    int X = convertCharToHex(secondNibble);  
    int Y = convertCharToHex(thirdNibble); 
    int spriteHeight = convertCharToHex(fourthNibble); 

    int originalCoordinateXVal = regist_V[X];
    int coordinateX = originalCoordinateXVal; 
    if(coordinateX > 63){
        originalCoordinateXVal = coordinateX % 64; 
        coordinateX = originalCoordinateXVal; 
    }

    int coordinateY = regist_V[Y];  
    if(coordinateY > 31){
        coordinateY %= 32; 
    }

    SDL_RenderSetScale(screen.renderer, screen.getScalingMultipiler(), screen.getScalingMultipiler());    
    uint16_t spriteDataAddress = regist_I; 

    for(int i = 0; i < spriteHeight; ++i){

        uint16_t binaryVal = memory.systemMemory[spriteDataAddress]; 
        bitset<16> binaryValue (binaryVal); 

        // TODO: add to debugger 
        // cout << "Binary value from memory address of register I " << binaryValue << endl; 

        // starting from 7 because LSB 
        for(int j = 7; j >= 0; --j){
            bool pixelIsPresent = screen.getPixelStatus(coordinateX, coordinateY);

            if(binaryValue[j] == 1 && pixelIsPresent == false && coordinateX <= 63 && coordinateY <= 31){
                // TODO: add custom renderer color, specifically this color as primary 
                SDL_SetRenderDrawColor(screen.renderer, 179, 254, 238, 1);     
                SDL_RenderDrawPoint(screen.renderer, coordinateX, coordinateY);   
                screen.setPixelStatus(coordinateX, coordinateY, binaryValue[j], cpu);
            }
            else if (binaryValue[j] == 1 && pixelIsPresent == true && coordinateX <= 63 && coordinateY <= 31){
                // TODO: set color as secondary 
                SDL_SetRenderDrawColor(screen.renderer, 0, 0, 0, 0);                
                SDL_RenderDrawPoint(screen.renderer, coordinateX, coordinateY);   
                screen.setPixelStatus(coordinateX, coordinateY, binaryValue[j], cpu);
            } 
            ++coordinateX; 
        }
        ++spriteDataAddress; 
        ++coordinateY; 
        // resets X coordinate for drawing the next line 
        coordinateX = originalCoordinateXVal;    
    }
    SDL_RenderPresent(screen.renderer);
}

// ex9e 
void Cpu::skipInstructionIfKeyIsPressed(char secondNibble, Keypad keypad){
    int X = convertCharToHex(secondNibble); 
   if(keypad.checkIfKeyIsPressed(regist_V[X])){
        incrementProgramCounter(getProgramCounterPointer(), 2); 
   }
}

// exa1 
void Cpu::skipInstructionIfKeyNotPressed(char secondNibble, Keypad keypad){
    int X = convertCharToHex(secondNibble); 
    if(keypad.checkIfKeyIsNotPressed(regist_V[X])){
        incrementProgramCounter(getProgramCounterPointer(), 2); 
    }
}

// fx07 
void Cpu::setVXToDelayTimer(char secondNibble){
    int X = convertCharToHex(secondNibble); 
    regist_V[X] = delayTimer; 
    cout << "At end of set delay timer " << endl; 
}

// fx0a 
void Cpu::loopUntilKeyPressed(char secondNibble, Screen& screen, Keypad keypad){
    int X = convertCharToHex(secondNibble); 
    uint8_t returnedResult = keypad.getPressedKey(screen); 

    if(returnedResult == '\0'){
        decrementProgramCounter(getProgramCounterPointer(), 2); 
    }
    else {
        regist_V[X] = returnedResult; 
    }
}

// fx1e 
void Cpu::addVXToRegisterI(char secondNibble, bool COSMAC_VIP_FLAG_IS_ON){
    int X = convertCharToHex(secondNibble); 
    regist_I += regist_V[X]; 

    if(debugger.getDebuggerIsOn() == true){
        debugger.outputIndexRegisterToDebugger(convertIntToHexString(regist_I)); 
    }

    if(COSMAC_VIP_FLAG_IS_ON == false){
        if (regist_I > 0x0FFF){ // 0x0FFF is regist_I's limit (0000-0FFF) 
            regist_V[0xF] = 1; 

            if(debugger.getDebuggerIsOn() == true){
                debugger.outputRegistersToDebugger(getRegist_V(0xF), 0xF);   
            }
        }
    }
}

// fx15 
void Cpu::setDelayTimerToVXValue(char secondNibble, Cpu& cpu){
    int X = convertCharToHex(secondNibble); 
    delayTimer = regist_V[X]; 

    delayTimerFuture = async(launch::async, &Cpu::runDelayTimer, &cpu); 
    cout << "Continuing asynchronously with delay timer thread " << endl; 
}

// fx29 
void Cpu::loadCharacterAddressInRegisterI(char secondNibble){
    int X = convertCharToHex(secondNibble); 
    int valueForVX; 

    if(regist_V[X] > 0xf){
        // to get last nibble 
        valueForVX = regist_V[X] % 16; 
    }
    else{
        valueForVX = regist_V[X]; 
    }

    // mutiply by 5 since each character's memory address is 5 apart 
    regist_I = 0x050 + (valueForVX * 5); 
}

// fx33 
void Cpu::storeEachVXDigitInMemory(char secondNibble, Memory& memory){
    int X = convertCharToHex(secondNibble); 
    uint8_t digitOfVX = 0; 
    uint8_t tempVXValue = regist_V[X]; 
    
    // adding 2 to compensate for modulus 10 and getting the right most digit  
    for(uint16_t currentAddress = regist_I + 2; currentAddress >= regist_I; --currentAddress){
        if (tempVXValue != 0){
            digitOfVX = tempVXValue % 10; 
            // storing float result in int to truncate decimal 
            tempVXValue /= 10; 
        }
        else {
            digitOfVX = 0; 
        }
        memory.systemMemory[currentAddress] = digitOfVX; 
    }
}  

// fx55 
void Cpu::storeRegistersToMemory(char secondNibble, Memory& memory, bool COSMAC_VIP_FLAG_IS_ON){
    int X = convertCharToHex(secondNibble); 
    uint16_t tempAddress = regist_I; 
    for(int i = 0; i <= X; ++i){
        memory.systemMemory[tempAddress] = regist_V[i]; 
        ++tempAddress; 
        
        if (COSMAC_VIP_FLAG_IS_ON == true){
            regist_I = tempAddress; 

            if(debugger.getDebuggerIsOn() == true){
                debugger.outputIndexRegisterToDebugger(convertIntToHexString(regist_I)); 
            } 
        }
    }
}  

// fx65 
void Cpu::storeMemoryToRegisters(char secondNibble, Memory memory, bool COSMAC_VIP_FLAG_IS_ON){
    int X = convertCharToHex(secondNibble); 
    uint16_t tempAddress = regist_I; 
    for(int i = 0; i <= X; ++i){
        regist_V[i] = memory.systemMemory[tempAddress]; 
        ++tempAddress; 
        
        if(debugger.getDebuggerIsOn() == true){
            debugger.outputRegistersToDebugger(getRegist_V(i), i);   
        }
        
        if (COSMAC_VIP_FLAG_IS_ON == true){
            regist_I = tempAddress; 

            if(debugger.getDebuggerIsOn() == true){
                debugger.outputIndexRegisterToDebugger(convertIntToHexString(regist_I)); 
            }
        }
    }
} 

void Cpu::fetchInstructions(Memory memory){

    stringstream instructionString;
    instructionString << hex << setw(2) << setfill('0') << (int)memory.systemMemory[getProgramCounter()];
    instructionString << hex << setw(2) << setfill('0') << (int)memory.systemMemory[getProgramCounter()+1];

    setCurrentInstruction(instructionString.str()); 
    
    if(debugger.getDebuggerIsOn() == true){
        debugger.outputCurrentInstructionToDebugger(getCurrentInstruction()); 
        debugger.outputProgramCounterToDebugger(getProgramCounter()); 
    }

    cout << "Pre-Increment Program Counter " << getProgramCounter() << endl; 
    incrementProgramCounter(getProgramCounterPointer(), 2); 
    cout << "Post-Increment Program Counter " << getProgramCounter() << endl; 
    cout << "Current Instruction " << getCurrentInstruction() << endl; 
}

void Cpu::decodeAndExecuteInstructions(string currentInstruction, Screen& screen, Memory& memory, Cpu& cpu, Keypad keypad){
    char firstNibble = currentInstruction[0]; 
    char secondNibble = currentInstruction[1]; 
    char thirdNibble = currentInstruction[2]; 
    char fourthNibble = currentInstruction[3]; 


        switch(firstNibble){
            case '0': 
                getCurrentInstruction(); // call function 
                switch (thirdNibble){
                    case 'e':    
                        switch (fourthNibble){
                            case '0':
                                // call clear screen function 00e0 
                                clearScreenInstruction(screen, cpu); 
                                break; 
                            case 'e':
                                // call return function 00ee
                                returnToAddressFromStack(memory); 
                                break; 
                        } 
                    default: 
                        break; 
                } 
            break;
            case '1':
                // 1nnn  
                // Note: n = the 12 bit address  
                jumpToAddress(getLastThreeNibbles(currentInstruction));  
                break;
            case '2': 
                // 2nnn 
                putAddressOnStack(getLastThreeNibbles(getCurrentInstruction()), memory); 
                break;
            case '3': 
                // 3xnn  
                skipInstructionIfVXEqualsNN(secondNibble, getLastTwoNibbles(getCurrentInstruction())); 
                break;
            case '4': 
                // 4xnn 
                skipInstructionIfVXNotEqualsNN(secondNibble, getLastTwoNibbles(getCurrentInstruction())); 
                break;
            case '5': 
                // 5xy0 
                skipInstructionIfVXEqualsVY(secondNibble, thirdNibble); 
                break;
            case '6': 
                // 600c 
                setValueInRegisterVX(secondNibble, getLastTwoNibbles(currentInstruction)); 
                break;
            case '7': 
                // call add value to register Vx function  
                addValueToRegisterVX(secondNibble, getLastTwoNibbles(currentInstruction)); 
                break;
            case '8': 
                switch (fourthNibble){
                    case '0':
                        // 8xy0 
                        setVXToValueOfVY(secondNibble, thirdNibble); 
                        break; 
                    case '1':
                        // 8xy1 
                        bitwiseOrVXAndVY(secondNibble, thirdNibble, COSMAC_VIP_FLAG_IS_ON); 
                        break; 
                    case '2':
                        // 8xy2 
                        bitwiseAndVXAndVY(secondNibble, thirdNibble, COSMAC_VIP_FLAG_IS_ON); 
                        break; 
                    case '3':
                        // 8xy3  
                        bitwiseExclusiveOrVXAndVY(secondNibble, thirdNibble, COSMAC_VIP_FLAG_IS_ON); 
                        break; 
                    case '4':
                        // 8xy4   
                        addVXToVY(secondNibble, thirdNibble); 
                        break; 
                    case '5':
                        // 8xy5   
                        subtractVYFromVX(secondNibble, thirdNibble); 
                        break; 
                    case '6':
                        // 8xy6   
                        shiftVXValueRight(secondNibble, thirdNibble, COSMAC_VIP_FLAG_IS_ON); 
                        break; 
                    case '7':
                        // 8xy7    
                        subtractVXFromVY(secondNibble, thirdNibble); 
                        break; 
                    case 'e':
                        // 8xyE 
                        shiftVXValueLeft(secondNibble, thirdNibble, COSMAC_VIP_FLAG_IS_ON); 
                        break; 
                } 
                break;
            case '9': 
                // 9xy0 
                skipInstructionIfVXNotEqualsVY(secondNibble, thirdNibble); 
                break;
            case 'a': 
                // annn 
                loadAddressInRegisterI(getLastThreeNibbles(currentInstruction));  
                break; 
            case 'b': 
                // bnnn/bxnn 
                jumpToAddressWithVXOffset(secondNibble, getLastThreeNibbles(currentInstruction)); 
                break;
            case 'c': 
                // cxnn 
                bitwiseANDRandNumAndNN(secondNibble, getLastTwoNibbles(currentInstruction)); 
                break;
            case 'd': 
                // dxyn 
                drawSpriteAtVXAndVY(secondNibble, thirdNibble, fourthNibble, screen, memory, cpu);  
                break;
            case 'e':       
                switch (thirdNibble){
                    case '9':
                        // ex9e 
                        skipInstructionIfKeyIsPressed(secondNibble, keypad);  
                        break; 
                    case 'a':
                        // exa1 
                        skipInstructionIfKeyNotPressed(secondNibble, keypad);  
                        break; 
                } 
                break;
            case 'f': 
                switch (thirdNibble){
                    case '0':
                        switch (fourthNibble){
                            case '7':
                                // fx07 
                                setVXToDelayTimer(secondNibble); 
                                break; 
                            case 'a':
                                // fx0a   
                                loopUntilKeyPressed(secondNibble, screen, keypad);     
                                break; 
                        } 
                        break;
                    case '1':
                        switch (fourthNibble){
                            case '5':
                                // fx15 
                                setDelayTimerToVXValue(secondNibble, cpu); 
                                break; 
                            case '8':
                                // fx18   
                                getCurrentInstruction();   
                                break; 
                            case 'e':
                                // fx1e  
                                addVXToRegisterI(secondNibble, COSMAC_VIP_FLAG_IS_ON); 
                                break;
                        }     
                        break; 
                    case '2':
                        // fx29 
                        loadCharacterAddressInRegisterI(secondNibble); 
                        break; 
                    case '3':
                        // fx33 
                        storeEachVXDigitInMemory(secondNibble, memory); 
                    break; 
                    case '5':
                        // fx55  
                        storeRegistersToMemory(secondNibble, memory, COSMAC_VIP_FLAG_IS_ON); 
                    break; 
                    case '6':
                        // fx65 
                        storeMemoryToRegisters(secondNibble, memory, COSMAC_VIP_FLAG_IS_ON); 
                    break; 
                } 
                break;
            default: 
                cout << "Status: (CPU) Error, Opcode not found " << endl; 
        }
}
 
int Cpu::convertCharToHex(char value){
    stringstream hexString; 
    hexString << "0x" << value;     
    return stoi(hexString.str(), nullptr, 16);    
}

string Cpu::convertIntToHexString(int value){
    stringstream convertedString; 
    convertedString << hex << value;     
    return convertedString.str();    
}
