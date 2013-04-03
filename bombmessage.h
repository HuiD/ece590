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
			stringstream ss;
			ss<<x;
			string strx=ss.str();
			stringstream ss1;
			ss1<<y;
			string stry=ss1.str();
			int pos=1;
			loadBuffer(strx, pos, buf);
			loadBuffer(stry, pos, buf);
			LoadBytes(buf, 8);
			finish();
		}
		void UnLoadByte(int&x, int&y, int& level)
		{
			
		}
};

