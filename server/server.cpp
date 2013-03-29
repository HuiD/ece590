#include "server.h"
server::server()
{
	Running=true;
	tcpclient=NULL;
	tcplistener=NULL;
	Connected=false;
}

int server::OnExecute()
{
	if(OnInit()==false)
	{
		return -1;
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
	tcplistener = new CHostSocket(1234);
	if(!tcplistener->Ok())
	{
		exit(EXIT_FAILURE);
	}
	tcpclient = new CClientSocket();
	return true;
}
void server::OnLoop() 
{
	if(!Connected)
	{
		if(tcplistener->Accept(*tcpclient))
		{
			Connected=true;
		}
		else{
			cout<<"waiting for connection"<<endl;
		}
	}
	if(tcpclient->Ready())
	{
		if(tcpclient->Receive(msg))
		{
			int playerID;
			int posX;
			int posY;
			msg.UnLoadByte(posX, posY, playerID);
			cout<<"X:"<<posX<<" Y:"<<posY<<"ID: "<<playerID;

		}
		else
			Connected=false;
	}
}

void server::OnCleanup()
{
	delete tcplistener;
	delete tcpclient;
	SDL_Quit();
}
int main(int argc, char* argv[])
{
	server s;
	return s.OnExecute();
}
