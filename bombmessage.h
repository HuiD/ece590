#include "CNet.h"
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace std;
class bombmessage : public CNetMessage
{
	private:
		virtual int NumToLoad()
		{
			if(state==EMPTY)
			{
				return 8;
			}
			else
			{
				return 0;
			}
		}
		virtual int NumToUnload()
		{
			if(state==FULL)
				return 8;
			else
				return 0;
		}
        void loadBuffer(string str, int& pos, charbuf& ch)
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
	public:
		void LoadByte(int x, int y, int level)
		{
			charbuf buf;
			buf[0]='b';
			buf[1]=level;
			stringstream ss;
			ss<<x;
			string strx=ss.str();
			stringstream ss1;
			ss1<<y;
			string stry=ss1.str();
			int pos=2;
			loadBuffer(strx, pos, buf);
			loadBuffer(stry, pos, buf);
			LoadBytes(buf, 8);
			finish();
		}
		virtual char getType()
		{
			return 'b';
		}
		void UnLoadByte(int&x, int&y, int& level)
		{
			
			charbuf buf;
			UnLoadBytes(buf);
			level=buf[1];
			char num[3];
			int pos=2;
			for(int i=0; i<3; i++)
			{
				num[i]=buf[pos++];
			}	
			x=atoi(num);
			char num1[3];
			for(int i=0; i<3; i++)
			{
				num1[i]=buf[pos++];
			}
			stringstream strv;
			strv<<num1;
			strv>>y;
		}
};

