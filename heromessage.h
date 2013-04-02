#include "CNet.h"
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace std;
class heromessage : public CNetMessage
{
	private: 
		virtual int NumToLoad();
		virtual int NumToUnload();
		void loadBuffer(string, int&, charbuf&);
	public:
		void LoadByte(hero_pos);
		void UnLoadByte(hero_pos&);
};
