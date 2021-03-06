#include "server.h"
server::server(int num)
{
	Running=true;
	tcpclient=NULL;
	tcplistener=NULL;
	Connected=false;
	numOfClients=0;
	max_players=num;
	for(int i=0; i<max_players; i++)
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
    net_thread=SDL_CreateThread(StaticThread,this);
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
	createBlocks();
	if(!tcplistener->Ok())
	{
		exit(EXIT_FAILURE);
	}
	for(int i=0; i<max_players; i++)
	{
		clients[i]=new CClientSocket();
	}
	return true;
}

void server::notifyClosed(int which)
{
	for(int i=0; i<max_players; i++)
	{
		if(i==which)
			continue;
		slotmessage slot;
		slot.LoadByte('2',which,0);
		if(clients[i])
		clients[i]->Send(slot);
	}
}

void server::createBlocks()
{
     for (int i=1; i<WINDOW_WIDTH/UNIT-1; i+=2){
        for (int j=3; j<WINDOW_HEIGHT/UNIT-1; j++){
            if (i==1&&j==3 || i==1&&j==4 || i==WINDOW_WIDTH/UNIT-2&&j==3 || i==WINDOW_WIDTH/UNIT-2&&j==4 || i==1&&j==WINDOW_HEIGHT/UNIT-2 || i==1&&j==WINDOW_HEIGHT/UNIT-3 || i==WINDOW_WIDTH/UNIT-2&&j==WINDOW_HEIGHT/UNIT-2 || i==WINDOW_WIDTH/UNIT-2&&j==WINDOW_HEIGHT/UNIT-3)
                continue;
            int r = rand()%2;
            if (r==0)
            {
                bpos blockpos;
                blockpos.x=i;
                blockpos.y=j;
                blockpos.r=rand()%9;
                blocks_pos.push_back(blockpos);
            }   
        }
    }
    for (int i=2; i<WINDOW_WIDTH/UNIT-1; i+=2){
        for (int j=3; j<WINDOW_HEIGHT/UNIT-1; j+=2){
            if (i==2&&j==3 || i==WINDOW_WIDTH/UNIT-3&&j==3 || i==2&&j==WINDOW_HEIGHT/UNIT-2 || i==WINDOW_WIDTH/UNIT-3&&j==WINDOW_HEIGHT/UNIT-2)
                continue;
            int r = rand()%2;
            if (r==0)
            {
                bpos blockpos;
                blockpos.x=i;
                blockpos.y=j;
                blockpos.r=rand()%9;
                blocks_pos.push_back(blockpos);
            } 
        }
    }  
    bpos blockpos;
    blockpos.x=0; blockpos.y=0; blockpos.r=9;
    blocks_pos.push_back(blockpos);
}

void server::sendOutBlockMessage(int which)
{
    for(int i=0; i<blocks_pos.size(); i++)
    {
        blockmessage bmsg;
        bmsg.LoadByte(blocks_pos[i].x, blocks_pos[i].y, blocks_pos[i].r);
        clients[which]->Send(bmsg);
    }
}

int server::net_thread_main()
{
    while(1)
    {
		for(int i=0; i<max_players; i++)
		{
			if(clients[i]==NULL)
				clients[i]=new CClientSocket();
			if(!clients[i]->Ok())
			{
				if(tcplistener->Accept(*clients[i]))
				{
					cout<<"tcp connected to slot: "<<i<<endl;
					sendOutBlockMessage(i);
				}
			}
			else if(clients[i]->Ready())
            {
                	slotmessage smsg;
					hellomessage hello;
					if(clients[i]->Receive(hello))
					{
                        int mport;
						int x;
						int y;
					    hello.UnLoadByte(mport, x, y);
					    Uint16 port=mport;	
					    clients[i]->setIp(port);
						smsg.LoadByte('0',i,max_players);
						clients[i]->Send(smsg);
						for(int j=0;j<max_players; j++)
						{
							if(j!=i&&clients[j]->Ok())
							{
								smsg.LoadByte('1', i, max_players);
								clients[j]->Send(smsg);
								smsg.LoadByte('1', j, max_players);
								clients[i]->Send(smsg);
							}
						}
                    }else
					{
						notifyClosed(i);
						cout<<"connection:"<<i<<"lost"<<endl;
						delete clients[i];
						clients[i]=NULL;
					}
            }
		}
  
    }
    return 0;
}

void server::OnLoop() 
{
		if(servsocket->Ready())
		{
			CNetMessage* msg;
			int channel;
			if(servsocket->Receive(msg, channel))
			{
				if(msg->getType()=='h')
				{
                    heromessage sentmsg;
				    hero_pos pos;
				    msg->UnLoadByte(pos.x,pos.y,pos.id);
                    for(int i=0; i<max_players; ++i)
                    {
                        if(clients[i]->Ok())
                        {
                            sentmsg.LoadByte(pos.x, pos.y, pos.id);
                            sendOutHeroMsg(sentmsg, channel, i);
                        }
                    }
				}else if(msg->getType()=='b')
                {
                    bombmessage bmsg;
                    int bx, by, lvl;
                    msg->UnLoadByte(bx, by, lvl);
					for(int i=0; i<max_players;++i)
					{
                    	if(clients[i]->Ok())
                    	{
                        	bmsg.LoadByte(bx,by,lvl);
                        	sendOutBombMsg(bmsg, channel, i);
                    	}
					}
                }

				delete msg;
			}
		}
}

void server::sendOutHeroMsg(heromessage msg, int channel, int i)
{
	servsocket->Send(msg, clients[i]->getIpAddress(), channel);
}

void server::sendOutBombMsg(bombmessage msg, int channel, int i)
{
    servsocket->Send(msg, clients[i]->getIpAddress(), channel);
}

void server::OnCleanup()
{
	delete tcplistener;
	delete tcpclient;
	delete servsocket;
	for(int i=0;i<max_players;i++)
	{
		if(clients[i]!=NULL)
		{
			delete clients[i];
			clients[i]=NULL;
		}
	}
	SDL_KillThread(net_thread); 
	SDL_WaitThread(net_thread,NULL); 
	SDLNet_Quit();
	SDL_Quit();
}
int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		cout<<"wrong arguments number\n";
		exit(EXIT_FAILURE);
	}
	server s(atoi(argv[1]));

	return s.OnExecute();
}
