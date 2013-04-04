#include "CNet.h"
#include<iostream>
#include "heromessage.h"
#include "bombmessage.h"
using namespace std;

bool CNet::Init() {
	if (SDLNet_Init() < 0) {
		std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
	  	return false;
        }
        else
        	return true;
}

void CNet::Quit() {
	SDLNet_Quit();
}


CNetMessage::CNetMessage() {
	reset();
}

void CNetMessage::reset() {
	for (int i =0; i < 256; i++)
		buffer [i] = 0;
	state = EMPTY;
}

void CNetMessage::finish() {
	if (state == READING)
		state = FULL;
}

void CNetMessage::LoadBytes(charbuf& inputbuffer, int n) {
	for (int i = 0; i < n; i++)
		buffer[i] = inputbuffer[i];
	state = READING;
}

void CNetMessage::UnLoadBytes(charbuf& destbuffer) {
	for (int i=0; i < this->NumToUnload(); i++)
		destbuffer[i] = buffer[i];
	reset();
}


CIpAddress::CIpAddress (Uint16 port) {
	if (SDLNet_ResolveHost(&m_Ip, NULL, port) < 0){
		std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
		m_Ip.host = 0;
		m_Ip.port = 0;
	}
}


CIpAddress::CIpAddress (std::string host, Uint16 port) {
	if (SDLNet_ResolveHost(&m_Ip, host.c_str(), port) < 0) {
		std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
		m_Ip.host = 0;
		m_Ip.port = 0;
	}
}

CIpAddress::CIpAddress() {
	m_Ip.host = 0;
	m_Ip.port = 0;
}

bool CIpAddress::Ok() const {
	return !(m_Ip.port == 0);
}

void CIpAddress::SetIp (IPaddress sdl_ip) { //sets a CTcpSocket object from a existing SDL socket
	m_Ip = sdl_ip;
}

IPaddress CIpAddress::GetIpAddress() const {
	return m_Ip;
}

Uint32 CIpAddress::GetHost() const {
	return m_Ip.host;
}

Uint16 CIpAddress::GetPort() const {
	return m_Ip.port;
}
void CIpAddress::setPort(Uint16 port) 
{
	m_Ip.port=port;
}



CTcpSocket::CTcpSocket() {
	m_Socket = NULL;
	set = SDLNet_AllocSocketSet(16);
}

CTcpSocket::~CTcpSocket() {
	if (!(m_Socket == NULL)) {
  		SDLNet_TCP_DelSocket(set,m_Socket);
		SDLNet_FreeSocketSet(set);
		SDLNet_TCP_Close (m_Socket);
		m_Socket=NULL;
	}
}

void CTcpSocket::SetSocket (TCPsocket the_sdl_socket) {
	if (!(m_Socket == NULL)) {
  		SDLNet_TCP_DelSocket(set,m_Socket);
  		SDLNet_TCP_Close (m_Socket);
  	}
	m_Socket = the_sdl_socket;
	if(SDLNet_TCP_AddSocket(set,m_Socket)==-1) {
		std::cerr << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << std::endl;
		m_Socket = NULL;
	}
}

bool CTcpSocket::Ok() const {
	return !(m_Socket == NULL);
}

bool CTcpSocket::Ready() const {
	bool rd = false;
	int numready = SDLNet_CheckSockets(set, 0);
	if (numready<0)
    		std::cerr << "SDLNet_CheckSockets: " << SDLNet_GetError() << std:: endl;
  	else
    		if (numready)
				rd = SDLNet_SocketReady (m_Socket);
  	return rd;
}

void CTcpSocket::OnReady() {

}

CHostSocket::CHostSocket (CIpAddress& the_ip_address) {
	CTcpSocket();
	IPaddress iph = the_ip_address.GetIpAddress();
	if (!(m_Socket = SDLNet_TCP_Open(&iph))) {
    		SDLNet_FreeSocketSet(set);
    		std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
	}else
    {
        SDLNet_TCP_AddSocket(set, m_Socket);
    }
}

CHostSocket::CHostSocket (Uint16 port) {
	CIpAddress iplistener (port);
	if (!iplistener.Ok()) {
		m_Socket = NULL;
	}
	else {
		CTcpSocket();
		IPaddress iph = iplistener.GetIpAddress();
		if (!(m_Socket = SDLNet_TCP_Open(&iph))) {
    		SDLNet_FreeSocketSet(set);
    		std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
		}else
        {
            SDLNet_TCP_AddSocket(set, m_Socket);
        }
	}
}


bool CHostSocket::Accept (CClientSocket& the_client_socket) {
	TCPsocket cs;
	if ((cs = SDLNet_TCP_Accept(m_Socket))) {
		the_client_socket.SetSocket(cs);
        if(SDLNet_TCP_AddSocket(set, cs)<0)
        {
            cout<<"maximum capacity reached"<<endl;
            return false;
        }
        else
        {
            return true;
        }
	}
	else
		return false;
}

void CHostSocket::OnReady() {

}


CClientSocket::CClientSocket() {
	CTcpSocket();
}
void CClientSocket::setPort(Uint16 port)
{
    m_RemoteIp.setPort(port);
}

CClientSocket::CClientSocket (std::string host, Uint16 port) {
	CIpAddress remoteip (host.c_str(), port);
	if (!remoteip.Ok()) {
		m_Socket = NULL;
	}
	else {
		CTcpSocket();
		Connect(remoteip);
	}
}

bool CClientSocket::Connect (CHostSocket& the_listener_socket) {
	return false;
}

bool CClientSocket::Connect (CIpAddress& remoteip) {
	TCPsocket cs;
	IPaddress ip = remoteip.GetIpAddress();
	if ((cs = SDLNet_TCP_Open(&ip)))
	{
		SetSocket(cs);
		return true;
	}
	else {
		std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
		return false;
	}
}



void CClientSocket::SetSocket (TCPsocket the_sdl_socket) {
	CTcpSocket::SetSocket (the_sdl_socket);
	IPaddress* ips;
	if ((ips = SDLNet_TCP_GetPeerAddress(m_Socket))) {
		m_RemoteIp.SetIp(*ips);
		Uint32 hbo = m_RemoteIp.GetHost();
		Uint16 pbo = m_RemoteIp.GetPort();
		std::cout << "Client connected: " << SDLNet_Read32(&hbo) << ' '
			<< SDLNet_Read16 (&pbo) << std::endl;
	}
	else
		std::cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << std::endl;
}

CIpAddress CClientSocket::getIpAddress () const {
	return m_RemoteIp;
}

bool CClientSocket::Receive(CNetMessage& rData) {
	if (m_Socket == NULL)
		return false;
	charbuf buf;

	while (rData.NumToLoad() > 0) {
		if (SDLNet_TCP_Recv(m_Socket, buf, rData.NumToLoad()) > 0) {
			rData.LoadBytes (buf, rData.NumToLoad());
		}
		else {
			return false;
		}
	}
	rData.finish();
	return true;
}

bool CClientSocket::Send (CNetMessage& sData) {
//check if there is a socket
	if (m_Socket == NULL)
		return false;
	charbuf buf;
	int len;

//Check if the instance can send bytes, if it can, unload the number of bytes specfied by NumToLoad() virtual function
	while ((len = sData.NumToUnload()) > 0) {
		sData.UnLoadBytes (buf);
		if (SDLNet_TCP_Send(m_Socket, (void *)buf, len) < len) {
			std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
			return false;
		}
        std::cerr<<"msg sent"<<endl;
	}
	return true;
}

void CClientSocket::setIp(std::string host, Uint16 port)
{
    CIpAddress tempip(host, port);
    m_RemoteIp=tempip;
}

void CClientSocket::OnReady() {

}


CUdpSocket::CUdpSocket(Uint16 port)
{
	udpsocket=NULL;
	for(int i=0; (udpsocket==NULL)&&i<10;++i)
	{
		udpsocket=SDLNet_UDP_Open(port+i);
        mport=port+i;
		cout<<"open port:"<<mport<<endl;
	}
	if(udpsocket==NULL)
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
	}
	if(!(udppacket = SDLNet_AllocPacket(256)))
	{
		fprintf(stderr, "SDLNet_AllocPacket Fail:%s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	set=SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(set, udpsocket);
	SDLNet_CheckSockets(set, 0);
}

CUdpSocket::~CUdpSocket()
{
	if(!(udpsocket==NULL))
	{
		SDLNet_UDP_DelSocket(set, udpsocket);
		SDLNet_FreeSocketSet(set);
		SDLNet_UDP_Close(udpsocket);
	}
}

void CUdpSocket::Send(CNetMessage& msg, CIpAddress ip, int channel)
{
	UDPpacket *p;
	if(udpsocket==NULL)
		return;
	charbuf buf;
	int len;
	while((len=msg.NumToUnload())>0)
	{
		if(!(p=SDLNet_AllocPacket(256)))
		{
			fprintf(stderr, "SDLNet_AllocPacket Fail:%s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		msg.UnLoadBytes(buf);
		memcpy(p->data, buf, p->maxlen);
		p->len=len+1;
	    p->address.host=ip.GetHost();	
		p->address.port=ip.GetPort();
		Uint32 ipa=ip.GetHost();
	}
	int sent=SDLNet_UDP_Send(udpsocket, -1, p);
	SDLNet_FreePacket(p);
}

bool CUdpSocket::Receive(CNetMessage* & msg, int & channel)
{
	charbuf buf;
	UDPpacket *p; 
	if(!(p=SDLNet_AllocPacket(256)))
	{
		exit(EXIT_FAILURE);
	}
	if(SDLNet_UDP_Recv(udpsocket, p))
	{
         if(p->data[0]=='h')
         {
             msg = new heromessage();
         }else
         {
             msg = new bombmessage();
         }
         channel=udppacket->channel;
		 memcpy(buf, p->data, p->maxlen); 	
		 msg->LoadBytes(buf, msg->NumToLoad());
	}else
	{
		 return false;
	}
	msg->finish();
	SDLNet_FreePacket(p);
	return true;
}
bool CUdpSocket:: Ready()
{
	bool rd=false;
	if(set==NULL){
		cout<<"set error"<<endl;
		exit(EXIT_FAILURE);
	}
	int numready=SDLNet_CheckSockets(set, 0);
	if(numready<0)
	{
		std::cerr<<"SDLNet_CheckSockets:"<<SDLNet_GetError()<<std::endl;
	}	
	else
		if(numready)
			rd=SDLNet_SocketReady(udpsocket);
	return rd;
}

Uint16 CUdpSocket::getPort()
{
    return mport;
}
