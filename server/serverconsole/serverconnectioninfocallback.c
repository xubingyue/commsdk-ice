#include "serverconnectioninfocallback.h"

void __stdcall serverConnectionInfoCallback(int type, const char* serverConnectionInfo)
{
	printf("%d, %s\n", type, serverConnectionInfo);
}
