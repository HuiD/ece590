#include "heromessage.h"

int heromessage::NumToLoad()
{
	if(state== EMPTY)
		return 8;
	else
		return 0;
}

int heromessage::NumToUnload()
{
	if(state==FULL)
		return 8;
	else
		return 0;
}

void heromessage::UnLoadByte(int&x, int& y, int& id)
{
    charbuf c;
    UnLoadBytes(c);
    id=(int)c[1];
	char num[3];
	int pos=2;
	for(int i=0; i<3; i++)
	{
		num[i]=c[pos++];
	}
	x=atoi(num);
	char num1[3];
	for(int i=0; i<3; i++)
	{
		num1[i]=c[pos++];
	}
	y=atoi(num1);
	cout<<"y pos :"<<y<<endl;
}

char heromessage::getType()
{
	return 'h';
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
void heromessage::LoadByte(int x, int y, int id)
{
	charbuf ch;
//	cout<<heropos.id<<endl;
    ch[0]='h';
	ch[1]=(char)id;
//	cout<<ch[0]<<endl;
	stringstream ss;
	ss<<x;
	string strx = ss.str();
	stringstream ss1;
	ss1<<y;
	string stry = ss1.str();
	int pos=2;
	loadBuffer(strx, pos, ch);
	loadBuffer(stry, pos, ch);
    LoadBytes(ch, 8);
	cout<<ch[5]<<ch[6]<<ch[7]<<": msg pos y"<<endl;
    finish();
}


