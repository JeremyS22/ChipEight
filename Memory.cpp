#include <iostream>
#include <fstream> 
#include <sstream>
#include <iomanip>

#include "Memory.h"
#include "Cpu.h"

using namespace std;  

void Memory::loadFontDataIntoMemory(Cpu cpu, Memory& memory){

    cout << "Status: (Memory) Loading font data into memory" << endl; 

    int tempMemLocation = 0x050; 
    
    for (int i = 0; i < 80; i++){ 
        memory.systemMemory[tempMemLocation]= cpu.fontData[i]; 
        tempMemLocation++; 
    }
    cout << "Status: (Memory) Font data loaded into memory" << endl; 
} 

void Memory::loadRomIntoMemory(Memory& memory, string romFileLocation, Cpu& cpu){    
    string romParser; 
    fstream rom; 
    rom.open(romFileLocation, ios::in); 
    if(!rom.is_open()){
        cout << "Status: (Memory) Could not read rom\n  Check file path" << endl; 
    }
    else {
        cout << "Status: (Memory) OPENED ROM" << endl; 

        ifstream file(romFileLocation, ios::binary | ios::in); 

        char lineOfRomData; 

        // prevents i from superceeding system's 4096 bytes of memory 
        for(int i = 0x200; file.get(lineOfRomData) && i < 4096; i++){
            cout << hex << setw(2) << setfill('0') << (int)(unsigned char)lineOfRomData << " \n"; 
            memory.systemMemory[i] = (uint8_t)lineOfRomData; 
        }

        cpu.setProgramCounter(cpu.getProgramCounterPointer(),0x200); 
        cout << "Status: (Memory) Contents read, Closing rom" << endl; 
        rom.close(); 
    }
}

// Use for debugging ------------------------------------------------------------------------------- 

void Memory::debug_printMemory(Memory memory, bool spaceEveryFourNibbles){
// Note: font data at 0x050, program data at 0x200 
// Note: 2000 set for brevity's sake, for loop range can be higher than 2000 (up to 4096)     
    for (int i = 0x050; i < 4096; i++){
        cout << "Address " << i << ": ";  
        cout << hex << setw(2) << setfill('0') << int(memory.systemMemory[i]) << " " << endl; 
        
        debug_addSpacing(i, spaceEveryFourNibbles); 
    }
} 

void Memory::debug_addSpacing(int i, bool spaceEveryFourNibbles){
    // to make memory containing cpu instructions easier to read 
    if(spaceEveryFourNibbles == true && i % 2 != 0){ 
        cout << "\n" << endl;  
    }
}
