#include "serverconnectioninfocallback.h"

void __stdcall serverConnectionInfoCallback(int type, const char* CServerConnectionInfo)
{
	printf("%d, %s\n", type, CServerConnectionInfo);
}
