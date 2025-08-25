all:
	g++ -g -I src/include -I src/include/SDL2 -L src/lib -o src/bin/ChipEight main.cpp Cpu.cpp Screen.cpp Memory.cpp Keypad.cpp Debugger.cpp ChipEight.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

run:
	./src/bin/ChipEight

clean:
	rm -f *.o ./src/bin/ChipEight.exe 