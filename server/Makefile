#Compliler flags
CPPFLAGS=-Wall -g `sdl-config --cflags`

#Objects
OBJECTS=server.o CNet.o heromessage.o

CC=g++

#Linker flags
LDFLAGS= `sdl-config --libs` -lSDL -lSDL_net

main: server.cpp
	$(CC) $(CPPFLAGS) -c server.cpp CNet.cpp heromessage.cpp
	$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o server

clean:
	rm -rf $(OBJECTS) server
