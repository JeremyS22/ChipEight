#include <iostream> 
#include <sstream> 
#include <cmath>

#include "Debugger.h" 

using namespace std; 

Debugger::Debugger() : 
    debuggerIsOn (false), 
    debuggingWindow (nullptr), 
    debuggingRenderer (nullptr), 
    fontRegular("./assets/Inter_28pt-Regular.ttf"), 
    fontExtraBold("./assets/Inter_28pt-ExtraBold.ttf"), 
    fontSemiBold("./assets/Inter_28pt-SemiBold.ttf"), 
    font(nullptr), 
    messageText(nullptr), 
    messageFont(nullptr),
    x(0), 
    y(0), 
    width(0), 
    height(0), 
    textIsStatic(false), 
    textColor(""){
        vector<std::string> pastInstructionVector(6,"0"); 
        vector<std::string> pastProgramCounterVector(6,"0"); 
        vector<std::string> stackPrintingVector(16,"0"); 
} 

bool Debugger::initializeDebugger(){

    SDL_Init(SDL_INIT_VIDEO); 
    SDL_Init(SDL_INIT_EVENTS); 

    SDL_CreateWindowAndRenderer(512, 512, (SDL_WINDOW_RESIZABLE), &debuggingWindow, &debuggingRenderer); 
    SDL_SetRenderDrawColor(debuggingRenderer, 0, 0, 0, 0);  
    SDL_RenderClear(debuggingRenderer); 
    SDL_SetWindowPosition(debuggingWindow, 0, 30); 
    SDL_SetWindowTitle(debuggingWindow, "Debugger"); 

    // returns 0 if successful, -1 on any error 
    if(TTF_Init() == -1){
        cout << "Error initializing debugging window, closing SDL and window" << endl; 
        return true; 
    }    

    createBoxAndAddText(fontSemiBold, "Registers", 10, 20, 70, 20, true, debuggingRenderer, "white"); 

    const char* registerNames[] = {"V0:", "V1:", "V2:", "V3:", "V4:", "V5:", "V6:", "V7:", "V8:", "V9:", "VA:", "VB:", "VC:", "VD:", "VE:", "VF:"}; 
    int height = 50;  
    for(int i = 0; i < 15; i+=2){
        createBoxAndAddText(fontRegular, registerNames[i], 10, height, 20, 20, true, debuggingRenderer, "white"); 
        createBoxAndAddText(fontRegular, registerNames[i+1], 90, height, 20, 20, true, debuggingRenderer, "white"); 
        height+=35; 
    }
    
    createBoxAndAddText(fontSemiBold, "PC:", 10, 330, 30, 20, true, debuggingRenderer, "white"); 
    createBoxAndAddText(fontSemiBold, "Instr:", 10, 350, 50, 20, true, debuggingRenderer, "white"); 
    createBoxAndAddText(fontSemiBold, "Stack:", 160, 20, 50, 20, true, debuggingRenderer, "white"); 

    createBoxAndAddText(fontRegular, "Register I:", 10, 372, 90, 22, true, debuggingRenderer, "white"); 

    createBoxAndAddText(fontRegular, "Delay:", 10, 395, 60, 22, true, debuggingRenderer, "white"); 

    SDL_RenderPresent(debuggingRenderer); 

    cout << "Status: (Debugger) Debugging Window and Renderer Created " << endl; 

    return false; 
}

bool Debugger::runDebugger(Cpu& cpu, Memory& memory, Screen& screen, Keypad& keypad, Debugger debugger){
    if (getDebuggerIsOn() == false){
        setDebuggerIsOn(true);
    }

    if(cpu.checkDelayTimerThreadIsRunning() == true){
        future<bool>& future = cpu.getFuture(); 
        future_status status; 

        while(true){
            cout << "waiting . . . " << endl; 
            this_thread::sleep_for(chrono::nanoseconds(10000000)); 
            status = future.wait_for(chrono::microseconds(1)); 

            if(status == future_status::ready){
                cout << "Future received in debugger " << endl; 
                break; 
            }
        }
    }

    resetDataOnDebuggerScreen(memory); 
    cout << "[DEBUGGER] Press right arrow key to step 1 instruction, up arrow key to step by 5 instructions" << endl; 
    cout << "[DEBUGGER]  Press Grave/Tilde key (AKA ` or ~) to leave debugger and resume emulation" << endl; 
    cout << "[DEBUGGER]   Press Escape key to close emulator" << endl; 
    while(screen.getWindowIsOpen()){
        if (SDL_PollEvent(&screen.windowEvent)){
            if(screen.windowEvent.type == SDL_QUIT){
                setDebuggerIsOn(false); 
                cout << "[DEBUGGER] Clicked closed, exiting debugging mode" << endl; 
                return true; 
            }
            else if (screen.windowEvent.type == SDL_KEYDOWN){
                switch (screen.windowEvent.key.keysym.sym){
                    case SDLK_BACKQUOTE: 
                        cout << "[DEBUGGER] Grave/Tilde key PRESSED, exiting debugging mode " << endl; 
                        setDebuggerIsOn(false); 
                        return false; 
                    case SDLK_RIGHT:
                        cpu.fetchInstructions(memory); 
                        cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad); 
                        break; 
                    case  SDLK_UP:
                        for (int i = 0; i < 5; ++i){
                            cpu.fetchInstructions(memory); 
                            cpu.decodeAndExecuteInstructions(cpu.getCurrentInstruction(), screen, memory, cpu, keypad);    
                        }
                        break; 
                    case SDLK_ESCAPE: 
                        setDebuggerIsOn(false); 
                        cout << "[DEBUGGER] ESC PRESSED, exiting emulation" << endl; 
                        return true; 
                    default:
                        break; 
                }
            }
        }
    }
    return false;   
}

void Debugger::setDebuggerIsOn(bool value){
    debuggerIsOn = value; 
}

bool Debugger::getDebuggerIsOn(){
    return debuggerIsOn; 
}

SDL_Renderer* Debugger::getDebuggingRenderer(){
    return debuggingRenderer; 
}

void Debugger::setStackPrintingVector(string value){
    stackPrintingVector.push_back(value); 
}

string Debugger::getStackPrintingVectorElement(int index){
    return stackPrintingVector.at(index); 
}

int Debugger::getStackPrintingVectorSize(){
    return stackPrintingVector.size(); 
}

bool Debugger::destroyDebuggerWindow(){
    SDL_DestroyWindow(debuggingWindow); 
    SDL_DestroyRenderer(debuggingRenderer); 
    TTF_CloseFont(messageFont); 
    messageFont = nullptr; 
    TTF_Quit(); 
    SDL_Quit();  
    
    return false; 
}

void Debugger::outputCurrentInstructionToDebugger(string instruction){
    createBoxAndAddText(fontExtraBold, instruction.c_str(), 90, 350, 50, 22, false, debuggingRenderer, "white");  
    outputPastInstructionsToDebugger(instruction); 
}

void Debugger::outputProgramCounterToDebugger(uint16_t programCounter){
    string convertedProgramCounter = convertIntToHexString(programCounter).c_str();  
    createBoxAndAddText(fontExtraBold, convertedProgramCounter.c_str(), 90, 329, 50, 22, false, debuggingRenderer, "white"); 
    outputPastPCToDebugger(convertedProgramCounter); 
}

void Debugger::outputPastInstructionsToDebugger(string currentInstruction){
    pastInstructionVector.emplace(pastInstructionVector.begin(), currentInstruction); 
    
    if(pastInstructionVector.size() > 6){
        pastInstructionVector.pop_back(); 
    }
        
    int xCoordinate = 150; 
    for(int i = 1; i < pastInstructionVector.size(); i++){
        if(i >= 4){
            createBoxAndAddText(fontExtraBold, pastInstructionVector[i].c_str(), xCoordinate, 350, 50, 22, false, debuggingRenderer, "dark gray");     
        }
        else {
            createBoxAndAddText(fontExtraBold, pastInstructionVector[i].c_str(), xCoordinate, 350, 50, 22, false, debuggingRenderer, "gray"); 
        }
        xCoordinate+=60; 
    }
}

void Debugger::outputPastPCToDebugger(string programCounter){
    pastProgramCounterVector.emplace(pastProgramCounterVector.begin(), programCounter); 

    if(pastProgramCounterVector.size() > 6){
        pastProgramCounterVector.pop_back(); 
    }
        
    int xCoordinate = 150; 
    for(int i = 1; i < pastProgramCounterVector.size(); i++){
        if(i >= 4){
            createBoxAndAddText(fontExtraBold, pastProgramCounterVector[i].c_str(), xCoordinate, 329, 50, 22, false, debuggingRenderer, "dark gray");     
        }
        else {
            createBoxAndAddText(fontExtraBold, pastProgramCounterVector[i].c_str(), xCoordinate, 329, 50, 22, false, debuggingRenderer, "gray"); 
        }
        xCoordinate+=60; 
    }
}

void Debugger::outputDelayTimerToDebugger(uint8_t delayTimer){
    createBoxAndAddText(fontExtraBold, convertIntToString(delayTimer, false, false).c_str(), 90, 396, 28, 22, false, debuggingRenderer, "white"); 
}

void Debugger::outputIndexRegisterToDebugger(string address){
    stringstream sstreamObj; 
    sstreamObj << "0x" << address;
    string convertedAddress = sstreamObj.str(); 
    createBoxAndAddText(fontExtraBold, convertedAddress.c_str(), 105, 372, 60, 22, false, debuggingRenderer, "white"); 
}

void Debugger::outputRegistersToDebugger(uint8_t registerValue, int registerName){
    string convertedRegisterValue = convertIntToString(registerValue, false, false); 

    // to get correct column based on register's name 
    int targetPositionX = (registerName % 2 == 0)  ? 40 : 120; 
    
    // to get correct row based on register's name 
    float tempVal = registerName / 2;
    int targetPositionY = (ceil(tempVal) * 35) + 50; 

    createBoxAndAddText(fontExtraBold, convertedRegisterValue.c_str(), targetPositionX, targetPositionY, 20, 22, false, debuggingRenderer, "white"); 
} 

void Debugger::outputStackToDebugger(Memory memory){
    if(memory.getStackSize() > stackPrintingVector.size()){
        string topOfStack = convertIntToHexString(memory.getStackPointer()); 
        stackPrintingVector.emplace(stackPrintingVector.begin(), topOfStack); 
    }
    else {
        // deletes text left on screen of popped off stack address  
        int printOverTextY = 50 + ((stackPrintingVector.size() - 1) * 35); 
        createBoxAndAddText(fontExtraBold, "", 190, printOverTextY, 110, 22, false, debuggingRenderer, "white"); 
        stackPrintingVector.erase(stackPrintingVector.begin()); 
    }

    int positionY = 50; 
    int secondPositionY = 50; 
    if(stackPrintingVector.size() == 0){ 
        createBoxAndAddText(fontRegular, "0x0", 190, positionY, 36, 22, false, debuggingRenderer, "white"); 
        createBoxAndAddText(fontRegular, "---", 240, positionY, 60, 22, false, debuggingRenderer, "white"); 
    }
    else {
        for(int i = 0; i < stackPrintingVector.size(); ++i){
            if(i > 7){
                createBoxAndAddText(fontRegular, convertIntToString(i, true, true).c_str(), 330, secondPositionY, 36, 22, false, debuggingRenderer, "white"); 
                createBoxAndAddText(fontExtraBold, stackPrintingVector.at(i).c_str(), 380, secondPositionY, 60, 22, false, debuggingRenderer, "white"); 
                secondPositionY+=35; 
            }
            else {
                createBoxAndAddText(fontRegular, convertIntToString(i, true, true).c_str(), 190, positionY, 36, 22, false, debuggingRenderer, "white"); 
                createBoxAndAddText(fontExtraBold, stackPrintingVector.at(i).c_str(), 240, positionY, 60, 22, false, debuggingRenderer, "white"); 
                positionY+=35; 
            }       
        }
    }
}

void Debugger::createBoxAndAddText(const char* font, const char* messageText, int x, int y, int width, int height, bool textIsStatic, SDL_Renderer* debuggingRenderer, 
    string textColor){

    SDL_Color pickedColor; 

    TTF_Font* messageFont = TTF_OpenFont(font, 28); 

    if(!messageFont){
        cout << "Error no TTF font not loaded, check your font file path " << endl;  
        return;        
    }

    if(textColor == "gray"){
        pickedColor = {128, 128, 128}; 
    }
    else if (textColor == "dark gray"){
        pickedColor = {64, 64, 64}; 
    }
    else {
        pickedColor = {255, 255, 255}; 
    }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(messageFont, messageText, pickedColor); 

    SDL_Texture* message = SDL_CreateTextureFromSurface(debuggingRenderer, surfaceMessage); 
    SDL_FreeSurface(surfaceMessage); 

    SDL_Rect messageTextbox; 
    messageTextbox.x = x; 
    messageTextbox.y = y; 
    messageTextbox.w = width; 
    messageTextbox.h = height; 

    if(textIsStatic == false){
        // drawing with renderer's color (black) over previous texture to "clear" it 
        SDL_RenderFillRect(debuggingRenderer, &messageTextbox); 
    }

    SDL_RenderCopy(debuggingRenderer, message, NULL, &messageTextbox); 
    
    SDL_RenderPresent(debuggingRenderer); 
    
    SDL_DestroyTexture(message);  

    TTF_CloseFont(messageFont); 
}

void Debugger::resetDataOnDebuggerScreen(Memory memory){
    resetRegisterData(); 
    resetPCAndInstructionData();     
    resetStackData(memory); 
    resetIndexRegisterData(); 
    resetDelayAndSoundTimerData(); 
}

void Debugger::resetRegisterData(){
    createBoxAndAddText(fontExtraBold, "", 40, 50, 20, 280, false, debuggingRenderer, "white"); 
    createBoxAndAddText(fontExtraBold, "", 120, 50, 20, 280, false, debuggingRenderer, "white"); 

    const char* printRegisterValAsZero = "0";
    int height = 50; 
    for(int i = 0; i < 15; i+=2){
        createBoxAndAddText(fontRegular, printRegisterValAsZero, 40, height, 18, 20, true, debuggingRenderer, "white"); 
        createBoxAndAddText(fontRegular, printRegisterValAsZero, 120, height, 18, 20, true, debuggingRenderer, "white");
        
        height+=35; 
    }
}

void Debugger::resetPCAndInstructionData(){
    pastInstructionVector.clear(); 
    pastProgramCounterVector.clear(); 

    createBoxAndAddText(fontExtraBold, "", 90, 329, 350, 44, false, debuggingRenderer, "white");  

    createBoxAndAddText(fontExtraBold, "0x0", 90, 329, 50, 22, false, debuggingRenderer, "white"); 
    createBoxAndAddText(fontExtraBold, "---", 90, 350, 50, 22, false, debuggingRenderer, "white");  
}

void Debugger::resetStackData(Memory memory){
    copyStackToDebuggingVector(memory); 
    createBoxAndAddText(fontExtraBold, "", 190, 50, 280, 276, false, debuggingRenderer, "white");
     
    createBoxAndAddText(fontRegular, "0x0", 190, 50, 36, 22, true, debuggingRenderer, "white"); 
    createBoxAndAddText(fontRegular, "---", 240, 50, 60, 22, true, debuggingRenderer, "white");  
}

void Debugger::copyStackToDebuggingVector(Memory memory){
    stackPrintingVector.clear(); 
    int stackSize = memory.systemStack.size(); 

    string topOfStack; 
    for(int i = 0; i < stackSize; ++i){
        topOfStack = convertIntToHexString(memory.systemStack.top()); 
        stackPrintingVector.push_back(topOfStack); 
        memory.systemStack.pop(); 
    }
}

void Debugger::resetIndexRegisterData(){
    createBoxAndAddText(fontExtraBold, "", 105, 372, 60, 22, false, debuggingRenderer, "white"); 

    createBoxAndAddText(fontExtraBold, "0x0", 105, 372, 60, 22, false, debuggingRenderer, "white"); 
}

void Debugger::resetDelayAndSoundTimerData(){
    createBoxAndAddText(fontExtraBold, "", 90, 396, 28, 22, false, debuggingRenderer, "white"); 
}

string Debugger::convertIntToString(int value, bool add0xPrefix, bool convertToHexDigits){
    stringstream stringStreamObj; 
    if (add0xPrefix == true){
        if(value > 9 && convertToHexDigits == true){
            value+=55; 
            stringStreamObj << "0x" << char(value); 
        }
        else {
            stringStreamObj << "0x" << value; 
        }
    }
    else {
        stringStreamObj << value; 
    }
    
    string convertedValue = stringStreamObj.str();    
    
    return  convertedValue; 
}

string Debugger::convertIntToHexString(int value){
    stringstream hexadecimalVal; 
    hexadecimalVal << "0x" << hex << value; 

    return hexadecimalVal.str(); 
}