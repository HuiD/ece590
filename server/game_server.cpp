#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_net.h"


void game::findInactiveSlot(int &which)
{
    for(which=0; which<MAX_CLIENTS; ++which)
    {
        if(people[which].sock&&!people[which].active)
        {
           	SDLNet_TCP_DelSocket(socketset, peopl[which].sock);
	        SDLNet_TCP_Close(people[which].sock);
            fprintf(stderr, "Killed inactive socket %d\n", which);
	        break; 
        }
    }
}

void game::addInactiveSocket(int which, TCPsocket newsock) {
    people[which].sock=newsock;
    people[which].peer = *SDLNet_TCP_GetPeerAddress(newsock);
    SDLNet_TCP_AddSocket(set, people[which].sock);
    fprintf(stderr, "New inactive socket %d\n", which);
}

void game::HandleServer(void)
{
    int which
    TCPsocket newsock;
    newsock = SDLNet_TCP_Accept(servsocket);
	if ( newsock == NULL ) {
		return;
	}

	for ( which=0; which<GAME_MAXPEOPLE; ++which ) {
		if ( ! people[which].sock ) {
			break;
		}
	}
	if ( which == MAX_CLIENTS ) {
		findInactivePersonSlot(which);
	}else
    {

		addInactiveSocket(which, newsock);
	}
}


void game::deleteConnection(int which)
{
	SDLNet_TCP_DelSocket(set, people[which].sock);
	SDLNet_TCP_Close(people[which].sock);
	people[which].sock=NULL;
}

void game::HandleClient(int which)
{
	char data[256];
	if(SDLNet_TCP_Recv(people[which].sock, data, 256)<=0)
	{
		deleteConnection(which);
	}
}

void game::sendOutUDPs(heromessage msg, int channel)
{
	UDPpacket *p;
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		if(!(p=SDLNet_AllocPacket(256)))
		{
			exit(EXIT_FAILURE);
		}
	    hero_pos pos;
		msg.UnLoadByte(pos);
		
	}
}

