#define MAX_CLIENTS 4
#include "heromessage.h"
struct Client{
    int active;
    TCPsocket sock;
    IPaddress peer;
    Uint8 name[256+1];
}

typedef Client client;


class game{
	private:
        client people[MAX_CLIENTS];
        bool finished=false;
        SDL_Thread *net_thread=NULL;
        TCPsocket servsocket=NULL;
        SDLNet_SocketSet set=NULL;
        UDPsocket udpSocket;
        UDPpacket *udpPacket;

        void findInactiveSlot(int &which);
        //void full(TCPsocket newsocket);
        void addInactiveSocket(int which, TCPsocket newsock);
        void HandleServer(void);
        //void sendNew(int about, int to);
        //void sendID(int which);
        void deleteConnection(int which);
        void HandleClient(int which);
        void sendOutUDPs(CNetMessage msg, int channel);
        void handleUDP();
        void checkIfFinished();
        void allocateSockets();
	public:
		game();
		int  OnExecute();
		bool OnInit();
		void OnLoop();
		void OnCleanup();

};
