#include <SDL.h>
#include "CNet.h"
#include "heromessage.h"
class server
{
	private:
		bool Connected;
		bool Running;
		CClientSocket* tcpclient;
		CHostSocket* tcplistener;
		heromessage msg;
	public:
		server();
		int  OnExecute();
		bool OnInit();
		void OnLoop();
		void OnCleanup();

};
