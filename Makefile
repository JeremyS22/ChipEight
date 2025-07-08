all:
	g++ -g -I src/include -L src/lib -o ChipEight main.cpp Cpu.cpp Screen.cpp Memory.cpp -lmingw32 -lSDL2main -lSDL2 
clean:
	rm -f *.o ChipEight.exe 