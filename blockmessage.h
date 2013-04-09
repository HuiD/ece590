#include "CNet.h"
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace std;

class blockmessage:public CNetMessage
{
	private:
		virtual int NumToLoad()
		{
			if(state==EMPTY)
				return 6;
			else
				return 0;
		}
		virtual int NumToUnload()
		{
			if(state==FULL)
				return 6;
			else
				return 0;
		}
	public:
		void LoadByte(int x, int y, int r)
		{
			charbuf ch;
			ch[0]='l';
			ch[1]=(char)(x/10);
			ch[2]=(char)(x%10);
			ch[3]=(char)(y/10);
			ch[4]=(char)(y%10);
			ch[5]=(char)r;
			LoadBytes(ch,6);
			finish();
		}
		void UnLoadByte(int &x, int &y, int &r)
		{
			charbuf ch;
			UnLoadBytes(ch);
			x=10*((int)ch[1])+(int)ch[2];
			y=10*((int)ch[3])+(int)ch[4];
			r=(int)ch[5];
		}

		char getType()
		{
			return 'l';
		}


};
