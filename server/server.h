#include <SDL.h>
#include "CNet.h"
#include "heromessage.h"
#include "hellomessage.h"
#include "slotmessage.h"
#include <SDL_net.h>
#define MAX_CLIENTS 2


class server
{
	private:
		bool Connected;
		bool Running;
		CClientSocket* tcpclient;
		CUdpSocket* servsocket;
		CHostSocket* tcplistener;
		CClientSocket* clients[MAX_CLIENTS];
		int numOfClients;
		hero_pos heroPos[MAX_CLIENTS];
		bool isReceived[MAX_CLIENTS];
		void sendOutHeroMsg(heromessage msg, int channel, int i);

    protected:
        static int StaticThread(void *param)
        {
            return ((server*)param)->net_thread_main();
        }
        int net_thread_main();
	public:
		server();
        
		int  OnExecute();
		bool OnInit();
		void OnLoop();
		void OnCleanup();

};
