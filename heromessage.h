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
		virtual char getType();
		void LoadByte(int x, int y, int id);
		void UnLoadByte(int &x, int&y, int& id);
};
