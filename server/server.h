#include <SDL.h>
#include "CNet.h"
#include <vector>
#include "heromessage.h"
#include "hellomessage.h"
#include "slotmessage.h"
#include "bombmessage.h"
#include "blockmessage.h"
#include <SDL_net.h>
#define MAX_CLIENTS 4
#define WINDOW_WIDTH 950
#define WINDOW_HEIGHT 750
#define UNIT 50

struct block_pos
{
    int x;
    int y;
    int r;
};
typedef block_pos bpos;

class server
{
	private:
		int max_players;
		bool Connected;
		bool Running;
		CClientSocket* tcpclient;
		CUdpSocket* servsocket;
		CHostSocket* tcplistener;
		CClientSocket* clients[MAX_CLIENTS];
        vector<bpos> blocks_pos;
		int numOfClients;
		hero_pos heroPos[MAX_CLIENTS];
		bool isReceived[MAX_CLIENTS];
		void sendOutHeroMsg(heromessage msg, int channel, int i);
        void sendOutBombMsg(bombmessage msg, int channel, int i);
        void sendOutBlockMessage(int which);
		void notifyClosed(int i);
        void createBlocks();

    protected:
        static int StaticThread(void *param)
        {
            return ((server*)param)->net_thread_main();
        }
        int net_thread_main();
	public:
		server();
		void setMaxPlayer(int num)
		{
			max_players=num;
		}		
		int  OnExecute();
		bool OnInit();
		void OnLoop();
		void OnCleanup();

};
