
void Memory::loadDataIntoMemory(uint8_t fontDataArray[], Memory& memory){

    cout << "Status: (CPU) Loading font data into memory" << endl; 

    int tempMemLocation = 0x050; 
    
    for (int i = 0; i < 80; i++){ 
        // cout << int(fontDataArray[i]) << endl; 
        memory.memory[tempMemLocation]= fontDataArray[i]; 
        tempMemLocation++; 
    }
}