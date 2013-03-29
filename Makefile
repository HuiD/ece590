#Compliler flags
CPPFLAGS=  -g `sdl-config --cflags`

#Objects
OBJECTS=main.o CNet.o heromessage.o

CC=g++

#Linker flags
LDFLAGS= `sdl-config --libs` -lSDL -lSDL_net

main: main.cpp
	$(CC) $(CPPFLAGS) -c main.cpp CNet.cpp heromessage.cpp
	$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o client

clean:
	rm -rf $(OBJECTS)
