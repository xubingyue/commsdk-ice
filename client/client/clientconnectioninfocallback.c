#include "clientconnectioninfocallback.h"

void __stdcall clientConnectionInfoCallback(int handle, int type, const char* CClientConnectionInfo)
{
	printf("%d, %d, %s\n", handle, type, CClientConnectionInfo);
	//printf("%d\n", type);
	//printf("%s\n", CClientConnectionInfo);
}
