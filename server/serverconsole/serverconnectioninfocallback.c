#include "serverconnectioninfocallback.h"

void __stdcall serverConnectionInfoCallback(int type, const char* connectionInfo)
{
	printf("\ncallback:\n%d, %s\n", type, connectionInfo);
}
