#include "Memory.h" 
#include <iostream>
#include <fstream> 
#include <sstream>
#include <iomanip>

using namespace std;  

void Memory::loadFontDataIntoMemory(uint8_t fontDataArray[], Memory& memory){

    cout << "Status: (CPU) Loading font data into memory" << endl; 

    int tempMemLocation = 0x050; 
    
    for (int i = 0; i < 80; i++){ 
        // cout << int(fontDataArray[i]) << endl; 
        memory.memory[tempMemLocation]= fontDataArray[i]; 
        tempMemLocation++; 
    }
} 

void Memory::loadRomIntoMemory(Memory& memory, string romFileLocation, uint16_t* programCounter){    
    string romParser; 
    fstream rom; 
    rom.open(romFileLocation, ios::in); 
    if(rom.is_open()){
        cout << "Status: (CPU) OPENED ROM" << endl; 

        int i = 0x200; 
            while (getline(rom, romParser)){

                istringstream iStreamObject(romParser);

                for (char& romData : romParser) {
                        // converts binary file data into hexadecimal 
                        cout << hex << setw(2) << setfill('0') << (int)(unsigned char)romData << " \n"; 
                        
                        memory.memory[i] = romData; 
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

void Memory::debug_printMemory(Memory memory, bool spaceEveryFourNibbles){
// Note: font data at 0x050, program data at 0x200 
    for (int i = 0x050; i < 1000; i++){
        cout << "Address " << i << ": ";  
        cout << hex << setw(2) << setfill('0') << int(memory.memory[i]) << " " << endl; 
        
        debug_addSpacing(i, spaceEveryFourNibbles); 
    }
} 

void Memory::debug_addSpacing(int i, bool spaceEveryFourNibbles){
    // to make memory containing cpu instructions easier to read 
    if(spaceEveryFourNibbles == true && i % 2 != 0){ 
        cout << "\n" << endl;  
    }
}
