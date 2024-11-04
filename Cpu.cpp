#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream> 
#include <iomanip> 

using namespace std; 

void loadDataIntoMemory(uint8_t fontDataArray[], uint8_t memory[]){

    cout << "Status: (CPU) Loading font data into memory" << endl; 

    int tempMemLocation = 0x050; 
    
    for (int i = 0; i < 80; i++){ 
        // cout << int(fontDataArray[i]) << endl; 
        memory[tempMemLocation]= fontDataArray[i]; 
        tempMemLocation++; 
    }

}

void loadRomIntoMemory(uint8_t memory[], string romFileLocation, uint16_t* programCounter){

    // TODO: Make programCounter argument pass by reference 
    
    string romParser; 
    fstream rom; 
    rom.open(romFileLocation, ios::in); 
    if(rom.is_open()){
        cout << "Status: (CPU) OPENED ROM" << endl; 

        int i = 0x200; 
            while (getline(rom, romParser)){

                istringstream iStreamObject(romParser);


                // memory[i] = romParser; 
                for (char& romData : romParser) {
                        // converts binary file data into hexadecimal 
                        cout << hex << setw(2) << setfill('0') << (int)(unsigned char)romData << " \n"; 
                        
                        memory[i] = romData; 
                        i++; 
                }
            }

        *programCounter = 0x200; 
        cout << "Status: (CPU) Contents read, Closing rom" << endl; 
        rom.close(); 
    }
    else
        cout << "Status: (CPU) Could not read rom\n  Check file path" << endl; 

}

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

void findRegisterOfNibble(char nibble, string lastTwoNibbles){
    if (nibble == '0'){
        regist_V0 = stoi(lastTwoNibbles, nullptr, 16); 

        cout << "REGISTER WITH THE NIBBLE " << regist_V0 << endl; 
    }
    else if (nibble == '1'){
        regist_V1 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '2'){
        regist_V2 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '3'){
        regist_V3 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '4'){
        regist_V4 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '5'){
        regist_V5 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '6'){
        regist_V6 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '7'){
        regist_V7 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '8'){
        regist_V8 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == '9'){
        regist_V9 = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == 'a'){
        regist_VA = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == 'b'){
        regist_VB = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == 'c'){
        regist_VC = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == 'd'){
        regist_VD = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == 'e'){
        regist_VE = stoi(lastTwoNibbles, nullptr, 16); 
    }
    else if (nibble == 'f'){
        regist_VF = stoi(lastTwoNibbles, nullptr, 16); 
    }

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

// 1nnn 
void jumpToAddress(string address){
    setProgramCounter(&programCounter, stoi(address, nullptr, 16));   
}


// 6xnn 
void setValueInRegisterX (char secondNibble, string value){
    
    findRegisterOfNibble(secondNibble, value); 
}

// annn 
void loadAddressInRegisterI(string address){
    cout << address << " HERE IS THE NUMBER VERSION OF THIS " << hex << setw(2) << setfill('0') << stoi(address, nullptr, 16) << endl; 
    regist_I = stoi(address, nullptr, 16); 
}

void fetchInstructions(uint8_t memory[]){

    stringstream instructionString;
    instructionString << hex << setw(2) << setfill('0') << (int)memory[getProgramCounter()];
    instructionString << hex << setw(2) << setfill('0') << (int)memory[getProgramCounter()+1];

    setCurrentInstruction(instructionString.str()); 

    incrementProgramCounter(&programCounter, 2); 
    cout << getProgramCounter() << endl; 

    printCurrentInstruction(instructionString.str());  
}

void decodeAndExecuteInstructions(string currentInstruction){
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
                // call set register Vx function 
                setValueInRegisterX(secondNibble, getLastTwoNibbles(currentInstruction)); 
                break;
            case '7': 
                getCurrentInstruction(); // call add value to register Vx function  
                break;
            case '8': 
                getCurrentInstruction(); // call function 
                break;
            case '9': 
                getCurrentInstruction(); // call function 
                break;
            case 'a': 
                // call set index to register I function  s
                loadAddressInRegisterI(getLastThreeNibbles(currentInstruction));  
                break; 
            case 'b': 
                getCurrentInstruction(); // call function 
                break;
            case 'c': 
                getCurrentInstruction(); // call function 
                break;
            case 'd': 
                getCurrentInstruction(); // call display/draw fucntion  
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
 

// Use for debugging ------------------------------------------------------------------------------- 

void printMemory(uint8_t memory[]){

// Note: font data at 0x050, program data at 0x200 
    for (int i = 0x050; i < 1000; i++){
                    cout << "Address " << i << ": ";  
                    cout << hex << setw(2) << setfill('0') << int(memory[i]) << " " << endl; 
                    
                   /*  if(i % 3 == 1)
                        cout << "\n" << endl;  */
    }
}

// TODO: Expand this to include other registers 
void printCurrentInstruction(string Instruction){
    cout << "Current Instruction: " << Instruction << endl; 
}
