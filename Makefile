game: main.cpp *.h
	g++ -I/usr/local/include/SDL main.cpp -o game -lSDL
clean:
	rm -f game *.o *~
