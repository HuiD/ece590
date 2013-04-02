#include "server.h"
server::server()
{
	Running=true;
	tcpclient=NULL;
	tcplistener=NULL;
	Connected=false;
	numOfClients=0;
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		isReceived[i]=false;
	}
}

int server::OnExecute()
{
	if(OnInit()==false)
	{
		return -1;
	}
    SDL_Thread * net_thread=SDL_CreateThread(StaticThread,this);
	if(!net_thread)
	{
		printf("SDL_CreateThread: %s\n",SDL_GetError());
	}
	while(Running)
	{
		OnLoop();
	}
	OnCleanup();
	return 0;
}

bool server::OnInit()
{

	if(SDLNet_Init()<0)
	{
		fprintf(stderr, "couldn't initialize sdl_net: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	tcplistener = new CHostSocket(1234);
	servsocket=new CUdpSocket(1234);
	if(!tcplistener->Ok())
	{
		exit(EXIT_FAILURE);
	}
	for(int i=0; i<4; i++)
	{
		clients[i]=new CClientSocket();
	}
	return true;
}

int server::net_thread_main()
{
    while(1)
    {
		for(int i=0; i<MAX_CLIENTS; i++)
		{
			if(!clients[i]->Ok())
			{
				if(tcplistener->Accept(*clients[i]))
				{
					cout<<"tcp connected to slot: "<<i<<endl;

				}
			}
			else if(clients[i]->Ready())
            {
                	slotmessage smsg;
					hellomessage hello;
					if(clients[i]->Receive(hello))
					{
                        Uint16 port;
					    hello.UnLoadByte(port);
					    clients[i]->setIp("localhost", port);
						smsg.LoadByte('0',i);
						clients[i]->Send(smsg);
						for(int j=0;j<MAX_CLIENTS; j++)
						{
							if(j!=i&&clients[j]->Ok())
							{
								smsg.LoadByte('1', i);
								clients[j]->Send(smsg);
								smsg.LoadByte('1', j);
								clients[i]->Send(smsg);
							}
						}
                    }
            }
		}
  
    }
    return 0;
}

void server::OnLoop() 
{
		//if(!clients[i]->Ok())
		//	continue;
		if(servsocket->Ready())
		{
			heromessage msg;
			int channel;
			if(servsocket->Receive(msg, channel))
			{
				heromessage sentmsg;
				hero_pos pos;
				msg.UnLoadByte(pos);
                for(int i=0; i<MAX_CLIENTS; ++i)
                {
                    if(clients[i]->Ok())
                    {
                        sentmsg.LoadByte(pos);
                        sendOutHeroMsg(sentmsg, channel, i);
                    }
                }
			}
		}
}

void server::sendOutHeroMsg(heromessage msg, int channel, int i)
{
	servsocket->Send(msg, clients[i]->getIpAddress(), channel);
	cout<<"send out udp to "<<endl;
}

void server::OnCleanup()
{
	delete tcplistener;
	delete tcpclient;
	for(int i=0; i<numOfClients; i++)
	{
		delete clients[i];
	}
	SDL_Quit();
}
int main(int argc, char* argv[])
{
	server s;

	return s.OnExecute();
}
