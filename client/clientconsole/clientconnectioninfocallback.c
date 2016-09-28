#include "clientconnectioninfocallback.h"

void __stdcall clientConnectionInfoCallback(int handle, int type, const char* clientConnectionInfo)
{
	printf("%d, %d, %s\n", handle, type, clientConnectionInfo);
}
