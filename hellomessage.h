#include "CNet.h"
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace std;
class hellomessage : public CNetMessage
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
		void LoadByte(int port, int a, int b)
		{
			charbuf buf;
			memcpy(&buf[0], &port,2);
			LoadBytes(buf, 2);
			finish();
		}

		void UnLoadByte(int& port, int& a, int& b)
		{
			charbuf buf;
			UnLoadBytes(buf);
			memcpy(&port, &buf[0],2);
		}
};
