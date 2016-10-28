#include "clientconnectioninfocallback.h"

void __stdcall clientConnectionInfoCallback(int index, int type,
        const char* connectionInfo)
{
    printf("\ncallback:\n%d, %d, %s\n", index, type, connectionInfo);
}
