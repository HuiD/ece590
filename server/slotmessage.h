#include "CNet.h"
#include <stdlib.h>
using namespace std;
class slotmessage:public CNetMessage
{
	private:
		virtual int NumToLoad()
		{
			if(state==EMPTY)
				return 3;
			else
				return 0;
		}
		virtual int NumToUnload()
		{
			if(state==FULL)
				return 3;
			else
				return 0;
		}
	public:
		char getType()
		{
			return 's';
		}
		void LoadByte(int ch, int id, int a)
		{
			charbuf buf;
			buf[0]=(char)ch;
			buf[1]=(char)id;
			buf[2]=(char)a;
			LoadBytes(buf,3);
			finish();
		}
		void UnLoadByte(int& ch, int& id, int& a)
		{
			charbuf buf;
			UnLoadBytes(buf);
			ch=(int)buf[0];
			id=(int)buf[1];
			a = (int)buf[2];
		}
};
