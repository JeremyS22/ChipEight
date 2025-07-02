all:
	g++ -g -I src/include -L src/lib -o ChipEight main.cpp -lmingw32 -lSDL2main -lSDL2 
clean:
	rm -f *.o ChipEight.exe 