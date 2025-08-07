all:
	g++ -g -I src/include -I src/include/SDL2 -L src/lib -o ChipEight main.cpp Cpu.cpp Screen.cpp Memory.cpp Keypad.cpp Debugger.cpp DebuggingTextbox.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
clean:
	rm -f *.o ChipEight.exe 