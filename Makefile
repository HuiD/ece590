#Compliler flags
CPPFLAGS=  -g `sdl-config --cflags`

#Objects
OBJECTS=main.o CNet.o heromessage.o

CC=g++

#Linker flags
LDFLAGS= -I/usr/local/include/SDL -lSDL -lSDL_mixer -lSDL_net -lSDL_ttf

main: main.cpp
	$(CC) $(CPPFLAGS) -c main.cpp CNet.cpp heromessage.cpp
		$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o client

clean:
	rm -rf $(OBJECTS) client
