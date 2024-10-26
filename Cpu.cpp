#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream> 
#include <iomanip> 

using namespace std; 

void initialize (){

}

void loadDataIntoMemory(uint8_t fontDataArray[], uint8_t memory[]){

    cout << "Status: Loading font data into memory" << endl; 

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
        cout << "Status: OPENED ROM" << endl; 

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
        cout << "Status: Contents read, Closing rom" << endl; 
        rom.close(); 
    }
    else
        cout << "Status: Could not read rom\n  Check file path" << endl; 

}
void incrementProgramCounter(uint16_t * programCounter, int value){
    *programCounter += value;  
}

// including this just in case, may delete this later 
void decrementProgramCounter(uint16_t * programCounter, int value){
    *programCounter -= value;  
}

uint16_t getProgramCounter (uint16_t * programCounter){
    return *programCounter; 
}

void setCurrentInstruction (string Instruction) {
    currentInstruction = Instruction; 
}

string getCurrentInstruction (){
    return currentInstruction; 
}

// TODO: Expand this to include other registers 
void printCurrentInstruction(string Instruction){
    cout << "Current Instruction: " << Instruction << endl; 
}

void fetchInstructions(uint8_t memory[]){

    stringstream instructionString;
    instructionString << hex << setw(2) << setfill('0') << (int)memory[getProgramCounter(&programCounter)];
    instructionString << hex << setw(2) << setfill('0') << (int)memory[getProgramCounter(&programCounter)+1];

    // TODO: check if this needs to be deleted, currentInstruction may need to be a string intead 
    // of int 
    // uint16_t instruction = stoi(instructionString.str());   
    
    // TODO: Check if this needs to be deleted, make string and use setter for currentInstruction 
    // setCurrentInstruction(instruction); 

    setCurrentInstruction(instructionString.str()); 

    incrementProgramCounter(&programCounter, 2); 
    cout << getProgramCounter(&programCounter) << endl; 

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
            getCurrentInstruction(); // call function 
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
            getCurrentInstruction(); // call set register Vx function 
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
            getCurrentInstruction(); // call set index to register I function  
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
            cout << "Status: Error, Opcode not found " << endl; 
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

