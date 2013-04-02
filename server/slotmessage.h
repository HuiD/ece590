#include "CNet.h"
#include <stdlib.h>
using namespace std;
class slotmessage:public CNetMessage
{
	private:
		virtual int NumToLoad()
		{
			if(state==EMPTY)
				return 2;
			else
				return 0;
		}
		virtual int NumToUnload()
		{
			if(state==FULL)
				return 2;
			else
				return 0;
		}
	public:
		void LoadByte(char ch, int id)
		{
			charbuf buf;
			buf[0]=ch;
			buf[1]=(char)id;
			LoadBytes(buf,3);
			finish();
		}
		void UnLoadByte(char& ch, int& id)
		{
			charbuf buf;
			UnLoadBytes(buf);
			ch=buf[0];
			id=(int)buf[1];
		}
};
