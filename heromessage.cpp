#include "heromessage.h"

int heromessage::NumToLoad()
{
	if(state== EMPTY)
		return 7;
	else
		return 0;
}

int heromessage::NumToUnload()
{
	if(state==FULL)
		return 7;
	else
		return 0;
}

void heromessage::UnLoadByte(int& posX, int& posY, int &ID)
{
    charbuf c;
    UnLoadBytes(c);
    ID=(int)c[0];
	char num[3];
	int pos=1;
	for(int i=0; i<3; i++)
	{
		num[i]=c[pos++];
	}
	posX=atoi(num);
	char num1[3];
	for(int i=0; i<3; i++)
	{
		num1[i]=c[pos++];
	}
	posY=atoi(num1);
 
   
  
}

void heromessage::loadBuffer(string str, int& pos, charbuf& ch)
{
	if(str.length()==1)
	{
		ch[pos++]='0';
		ch[pos++]='0';
		ch[pos++]=str.at(0);
	}
	else if(str.length()==2)
	{
		ch[pos++]='0';
		ch[pos++]=str.at(0);
		ch[pos++]=str.at(1);
	}
	else if(str.length()==3)
	{
		ch[pos++]=str.at(0);
		ch[pos++]=str.at(1);
		ch[pos++]=str.at(2);
	}
		
}
void heromessage::LoadByte(int posX, int posY, int playerID)
{
	charbuf ch;
	ch[0]=(char)playerID;
	stringstream ss;
	ss<<posX;
	string strx = ss.str();
	stringstream ss1;
	ss1<<posY;
	string stry = ss1.str();
	int pos=1;
	loadBuffer(strx, pos, ch);
	loadBuffer(stry, pos, ch);	
    LoadBytes(ch, 7);
    finish();
}


